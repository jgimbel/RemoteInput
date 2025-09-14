'use strict';

import fs from 'fs';
import path from 'path';

// Constants (avoid magic values)
const DEFAULT_PROCESS_NAME = process.env.TARGET_PROCESS_NAME || 'RuneLite.exe';
const FALLBACK_PROCESS_NAME = process.env.TARGET_FALLBACK_PROCESS_NAME || 'javaw.exe';
const TARGET_PID_ENV = process.env.TARGET_PID;

// function resolveLocalDll() {
//   const candidates = [
//     path.join(__dirname, '..', 'build', 'core', 'libRemoteInput.dll'),
//     path.join(__dirname, '..', 'build', 'core', 'Release', 'libRemoteInput.dll'),
//     path.join(__dirname, '..', 'build', 'core', 'Debug', 'libRemoteInput.dll'),
//     path.join(__dirname, '..', 'libRemoteInput.dll'),
//     path.join(process.cwd(), 'build', 'core', 'libRemoteInput.dll'),
//     path.join(process.cwd(), 'build', 'core', 'Release', 'libRemoteInput.dll'),
//     path.join(process.cwd(), 'build', 'core', 'Debug', 'libRemoteInput.dll'),
//   ];
//   for (const p of candidates) {
//     try {
//       if (fs.existsSync(p)) return p;
//     } catch (_) {}
//   }
//   return null;
// }

// (function ensureDllEnv() {
//   if (!process.env.REMOTE_INPUT_DLL) {
//     const guess = resolveLocalDll();
//     if (guess) {
//       process.env.REMOTE_INPUT_DLL = guess;
//       console.log(`[info] REMOTE_INPUT_DLL not set; using ${guess}`);
//     } else {
//       console.log('[warn] REMOTE_INPUT_DLL not set and no local DLL found. Relying on system PATH.');
//     }
//   }
// })();

// Load addon from package root
import ri from '../src/index';

console.log(Object.keys(ri));

function enumKeyByValue(obj: Record<string, any>, val: any): string {
  return Object.keys(obj).find(k => obj[k] === val) || String(val);
}

function logStep(title: string, data?: any) {
  console.log(`\n=== ${title} ===`);
  if (data !== undefined) console.log(data);
}

function choosePid(pids: number[] | null): number | undefined {
  if (TARGET_PID_ENV && !Number.isNaN(Number(TARGET_PID_ENV))) return Number(TARGET_PID_ENV);
  return pids && pids.length > 0 ? pids[0] : undefined;
}

function tryInject(name: string) {
  try {
    ri.EIOS.inject(name);
    console.log(`[ok] inject(${name})`);
  } catch (e) {
    console.log(`[warn] inject(${name}) failed or not needed:`, (e as Error)?.message || e);
  }
}

function main() {
  logStep('ImageFormat enum', ri.ImageFormat);

  // Discover clients before inject
  let pids = ri.EIOS.getClientsPids(false);
  logStep('Clients (pre-inject)', pids);

  // Try injection for desired names
  tryInject(DEFAULT_PROCESS_NAME);
  if (!pids?.length) {
    tryInject(FALLBACK_PROCESS_NAME);
  }

  // Discover clients again
  pids = ri.EIOS.getClientsPids(false);
  logStep('Clients (post-inject)', pids);

  const pid = choosePid(pids);
  if (!pid) {
    console.error('[error] No client PID found. Set TARGET_PID or ensure the target is running and injectable.');
    process.exitCode = 2;
    return;
  }
  console.log(`[info] Using PID ${pid}`);

  // Pair / get instance
  let e = ri.EIOS.fromPid(pid);
  if (!e) {
    e = ri.EIOS.pairClientPid(pid);
  }
  if (!e) {
    console.error('[error] Unable to obtain EIOS instance for PID.');
    process.exitCode = 3;
    return;
  }

  logStep('EIOS properties', { pid: e.pid });

  // Focus
  const initialFocus = e.hasFocus();
  try { e.gainFocus(); } catch (_) {}
  const afterGain = e.hasFocus();
  logStep('Focus', { initialFocus, afterGain });

  // Flags
  const flags = {
    kbdEnabled: e.isKeyboardInputEnabled(),
    mouseEnabled: e.isMouseInputEnabled(),
  };
  logStep('Input flags', flags);

  // Dimensions and buffers
  const dim = e.getTargetDimensions();
  const fmt = e.getImageFormat();
  const fmtName = enumKeyByValue(ri.ImageFormat, fmt);
  const buf = e.getImageBuffer();
  const expectedLen = dim && dim.width && dim.height ? dim.width * dim.height * 4 : undefined;
  logStep('Graphics', {
    dim,
    imageFormat: { value: fmt, name: fmtName },
    bufferLen: buf?.byteLength,
    expectedLen,
  });

  try { e.setGraphicsDebugging(true); } catch (_) {}
  const dbg = e.getDebugImageBuffer();
  console.log('Debug buffer len:', dbg?.byteLength);

  // Mouse
  const mouse = e.getMousePosition();
  logStep('Mouse position', mouse);
  try {
    // Nudge by 1px and revert
    if (typeof mouse?.x === 'number' && typeof mouse?.y === 'number') {
      e.moveMouse(mouse.x + 1, mouse.y + 1);
    }
    const mouse2 = e.getMousePosition();
    logStep('Mouse position after move', mouse2);
  } catch (err) {
    console.log('[warn] moveMouse failed:', (err as Error)?.message || err);
  }

  // Keyboard
  try {
    const kb = {
      speed: e.getKeyboardSpeed(),
      repeatDelay: e.getKeyboardRepeatDelay(),
    };
    logStep('Keyboard settings', kb);
    e.sendString('test', 0, 0);
    console.log('[ok] sendString("test")');
  } catch (err) {
    console.log('[warn] Keyboard ops failed:', (err as Error)?.message || err);
  }

  console.log('\nAll checks done.');
}

try {
  main();
} catch (err) {
  console.error('[fatal] Unexpected error:', (err as Error)?.stack || (err as Error)?.message || err);
  process.exitCode = 1;
}