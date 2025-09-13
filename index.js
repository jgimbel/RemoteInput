const path = require('path');

const buildType = process.env.DEBUG ? 'Debug' : 'Release';
const addonPath = path.join(__dirname, 'build', buildType, 'remote_input.node');

console.log(`Loading RemoteInput addon from: ${addonPath}`);
const addon = require(addonPath);

// Attach JS enums to avoid magic numbers in user code
// MouseButton mapping follows core (Simba-style): Left=1, Right=0, Middle=2
addon.MouseButton = Object.freeze({
  Left: 1,
  Right: 0,
  Middle: 2,
});

// Common Windows Virtual-Key codes for keyboard operations
// Letters/digits use ASCII codes; include common controls and arrows
addon.KeyCode = Object.freeze({
  SHIFT: 0x10,
  LSHIFT: 0xA0,
  RSHIFT: 0xA1,
  CONTROL: 0x11,
  LCONTROL: 0xA2,
  RCONTROL: 0xA3,
  LALT: 0xA4,
  RALT: 0xA5,
  LEFT_WINDOWS: 0x5B,
  RIGHT_WINDOWS: 0x5C,
  APPS: 0x5D,
  TAB: 0x09,
  BACKSPACE: 0x08,
  ENTER: 0x0D,
  ESCAPE: 0x1B,
  SPACE: 0x20,

  LEFT: 0x25,
  UP: 0x26,
  RIGHT: 0x27,
  DOWN: 0x28,

  DIGIT_0: 0x30,
  DIGIT_1: 0x31,
  DIGIT_2: 0x32,
  DIGIT_3: 0x33,
  DIGIT_4: 0x34,
  DIGIT_5: 0x35,
  DIGIT_6: 0x36,
  DIGIT_7: 0x37,
  DIGIT_8: 0x38,
  DIGIT_9: 0x39,

  KEY_A: 0x41,
  KEY_B: 0x42,
  KEY_C: 0x43,
  KEY_D: 0x44,
  KEY_E: 0x45,
  KEY_F: 0x46,
  KEY_G: 0x47,
  KEY_H: 0x48,
  KEY_I: 0x49,
  KEY_J: 0x4A,
  KEY_K: 0x4B,
  KEY_L: 0x4C,
  KEY_M: 0x4D,
  KEY_N: 0x4E,
  KEY_O: 0x4F,
  KEY_P: 0x50,
  KEY_Q: 0x51,
  KEY_R: 0x52,
  KEY_S: 0x53,
  KEY_T: 0x54,
  KEY_U: 0x55,
  KEY_V: 0x56,
  KEY_W: 0x57,
  KEY_X: 0x58,
  KEY_Y: 0x59,
  KEY_Z: 0x5A,

  F1: 0x70,
  F2: 0x71,
  F3: 0x72,
  F4: 0x73,
  F5: 0x74,
  F6: 0x75,
  F7: 0x76,
  F8: 0x77,
  F9: 0x78,
  F10: 0x79,
  F11: 0x7A,
  F12: 0x7B,

  NUM_LOCK: 0x90,
});

module.exports = addon;