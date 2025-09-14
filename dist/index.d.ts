interface EIOS {
    inject(processName: string): void;
    pairClientPid(pid: number): EIOS | null;
    fromPid(pid: number): EIOS | null;
    getClientsPids(onlyInjectable: boolean): number[] | null;
    listProcesses(): string[];
    getMousePosition(): {
        x: number;
        y: number;
    };
    setMousePosition(x: number, y: number): void;
    sendMouseButton(button: number, isDown: boolean): void;
    sendMouseWheel(delta: number): void;
    sendKey(keyCode: number, isDown: boolean): void;
    sendString(text: string, delayMs?: number, flags?: number): void;
    getKeyboardSpeed(): number;
    getKeyboardRepeatDelay(): number;
    isKeyboardInputEnabled(): boolean;
    isMouseInputEnabled(): boolean;
    enableKeyboardInput(enable: boolean): void;
    enableMouseInput(enable: boolean): void;
    getTargetDimensions(): {
        width: number;
        height: number;
    };
    getImageFormat(): number;
    getImageBuffer(): ArrayBuffer | null;
    setGraphicsDebugging(enable: boolean): void;
    getDebugImageBuffer(): ArrayBuffer | null;
    hasFocus(): boolean;
    gainFocus(): void;
    moveMouse(x: number, y: number): void;
    pid: number;
}
export declare enum ImageFormat {
    BGR_BGRA = 0,
    BGRA = 1,
    RGBA = 2,
    ARGB = 3,
    ABGR = 4
}
export declare enum MouseButton {
    Left = 1,
    Right = 0,
    Middle = 2
}
export declare enum KeyCode {
    SHIFT = 16,
    LSHIFT = 160,
    RSHIFT = 161,
    CONTROL = 17,
    LCONTROL = 162,
    RCONTROL = 163,
    LALT = 164,
    RALT = 165,
    LEFT_WINDOWS = 91,
    RIGHT_WINDOWS = 92,
    APPS = 93,
    TAB = 9,
    BACKSPACE = 8,
    ENTER = 13,
    ESCAPE = 27,
    SPACE = 32,
    LEFT = 37,
    UP = 38,
    RIGHT = 39,
    DOWN = 40,
    DIGIT_0 = 48,
    DIGIT_1 = 49,
    DIGIT_2 = 50,
    DIGIT_3 = 51,
    DIGIT_4 = 52,
    DIGIT_5 = 53,
    DIGIT_6 = 54,
    DIGIT_7 = 55,
    DIGIT_8 = 56,
    DIGIT_9 = 57,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    F1 = 112,
    F2 = 113,
    F3 = 114,
    F4 = 115,
    F5 = 116,
    F6 = 117,
    F7 = 118,
    F8 = 119,
    F9 = 120,
    F10 = 121,
    F11 = 122,
    F12 = 123,
    NUM_LOCK = 144
}
declare const addon: {
    EIOS: EIOS;
    MouseButton: Readonly<Record<string, MouseButton>>;
    KeyCode: Record<string, KeyCode>;
    ImageFormat: Readonly<Record<string, ImageFormat>>;
};
export default addon;
