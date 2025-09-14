"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.KeyCode = exports.MouseButton = exports.ImageFormat = void 0;
const path_1 = __importDefault(require("path"));
var ImageFormat;
(function (ImageFormat) {
    ImageFormat[ImageFormat["BGR_BGRA"] = 0] = "BGR_BGRA";
    ImageFormat[ImageFormat["BGRA"] = 1] = "BGRA";
    ImageFormat[ImageFormat["RGBA"] = 2] = "RGBA";
    ImageFormat[ImageFormat["ARGB"] = 3] = "ARGB";
    ImageFormat[ImageFormat["ABGR"] = 4] = "ABGR";
})(ImageFormat || (exports.ImageFormat = ImageFormat = {}));
var MouseButton;
(function (MouseButton) {
    MouseButton[MouseButton["Left"] = 1] = "Left";
    MouseButton[MouseButton["Right"] = 0] = "Right";
    MouseButton[MouseButton["Middle"] = 2] = "Middle";
})(MouseButton || (exports.MouseButton = MouseButton = {}));
var KeyCode;
(function (KeyCode) {
    KeyCode[KeyCode["SHIFT"] = 16] = "SHIFT";
    KeyCode[KeyCode["LSHIFT"] = 160] = "LSHIFT";
    KeyCode[KeyCode["RSHIFT"] = 161] = "RSHIFT";
    KeyCode[KeyCode["CONTROL"] = 17] = "CONTROL";
    KeyCode[KeyCode["LCONTROL"] = 162] = "LCONTROL";
    KeyCode[KeyCode["RCONTROL"] = 163] = "RCONTROL";
    KeyCode[KeyCode["LALT"] = 164] = "LALT";
    KeyCode[KeyCode["RALT"] = 165] = "RALT";
    KeyCode[KeyCode["LEFT_WINDOWS"] = 91] = "LEFT_WINDOWS";
    KeyCode[KeyCode["RIGHT_WINDOWS"] = 92] = "RIGHT_WINDOWS";
    KeyCode[KeyCode["APPS"] = 93] = "APPS";
    KeyCode[KeyCode["TAB"] = 9] = "TAB";
    KeyCode[KeyCode["BACKSPACE"] = 8] = "BACKSPACE";
    KeyCode[KeyCode["ENTER"] = 13] = "ENTER";
    KeyCode[KeyCode["ESCAPE"] = 27] = "ESCAPE";
    KeyCode[KeyCode["SPACE"] = 32] = "SPACE";
    KeyCode[KeyCode["LEFT"] = 37] = "LEFT";
    KeyCode[KeyCode["UP"] = 38] = "UP";
    KeyCode[KeyCode["RIGHT"] = 39] = "RIGHT";
    KeyCode[KeyCode["DOWN"] = 40] = "DOWN";
    KeyCode[KeyCode["DIGIT_0"] = 48] = "DIGIT_0";
    KeyCode[KeyCode["DIGIT_1"] = 49] = "DIGIT_1";
    KeyCode[KeyCode["DIGIT_2"] = 50] = "DIGIT_2";
    KeyCode[KeyCode["DIGIT_3"] = 51] = "DIGIT_3";
    KeyCode[KeyCode["DIGIT_4"] = 52] = "DIGIT_4";
    KeyCode[KeyCode["DIGIT_5"] = 53] = "DIGIT_5";
    KeyCode[KeyCode["DIGIT_6"] = 54] = "DIGIT_6";
    KeyCode[KeyCode["DIGIT_7"] = 55] = "DIGIT_7";
    KeyCode[KeyCode["DIGIT_8"] = 56] = "DIGIT_8";
    KeyCode[KeyCode["DIGIT_9"] = 57] = "DIGIT_9";
    KeyCode[KeyCode["KEY_A"] = 65] = "KEY_A";
    KeyCode[KeyCode["KEY_B"] = 66] = "KEY_B";
    KeyCode[KeyCode["KEY_C"] = 67] = "KEY_C";
    KeyCode[KeyCode["KEY_D"] = 68] = "KEY_D";
    KeyCode[KeyCode["KEY_E"] = 69] = "KEY_E";
    KeyCode[KeyCode["KEY_F"] = 70] = "KEY_F";
    KeyCode[KeyCode["KEY_G"] = 71] = "KEY_G";
    KeyCode[KeyCode["KEY_H"] = 72] = "KEY_H";
    KeyCode[KeyCode["KEY_I"] = 73] = "KEY_I";
    KeyCode[KeyCode["KEY_J"] = 74] = "KEY_J";
    KeyCode[KeyCode["KEY_K"] = 75] = "KEY_K";
    KeyCode[KeyCode["KEY_L"] = 76] = "KEY_L";
    KeyCode[KeyCode["KEY_M"] = 77] = "KEY_M";
    KeyCode[KeyCode["KEY_N"] = 78] = "KEY_N";
    KeyCode[KeyCode["KEY_O"] = 79] = "KEY_O";
    KeyCode[KeyCode["KEY_P"] = 80] = "KEY_P";
    KeyCode[KeyCode["KEY_Q"] = 81] = "KEY_Q";
    KeyCode[KeyCode["KEY_R"] = 82] = "KEY_R";
    KeyCode[KeyCode["KEY_S"] = 83] = "KEY_S";
    KeyCode[KeyCode["KEY_T"] = 84] = "KEY_T";
    KeyCode[KeyCode["KEY_U"] = 85] = "KEY_U";
    KeyCode[KeyCode["KEY_V"] = 86] = "KEY_V";
    KeyCode[KeyCode["KEY_W"] = 87] = "KEY_W";
    KeyCode[KeyCode["KEY_X"] = 88] = "KEY_X";
    KeyCode[KeyCode["KEY_Y"] = 89] = "KEY_Y";
    KeyCode[KeyCode["KEY_Z"] = 90] = "KEY_Z";
    KeyCode[KeyCode["F1"] = 112] = "F1";
    KeyCode[KeyCode["F2"] = 113] = "F2";
    KeyCode[KeyCode["F3"] = 114] = "F3";
    KeyCode[KeyCode["F4"] = 115] = "F4";
    KeyCode[KeyCode["F5"] = 116] = "F5";
    KeyCode[KeyCode["F6"] = 117] = "F6";
    KeyCode[KeyCode["F7"] = 118] = "F7";
    KeyCode[KeyCode["F8"] = 119] = "F8";
    KeyCode[KeyCode["F9"] = 120] = "F9";
    KeyCode[KeyCode["F10"] = 121] = "F10";
    KeyCode[KeyCode["F11"] = 122] = "F11";
    KeyCode[KeyCode["F12"] = 123] = "F12";
    KeyCode[KeyCode["NUM_LOCK"] = 144] = "NUM_LOCK";
})(KeyCode || (exports.KeyCode = KeyCode = {}));
let cModule = null;
try {
    const addonPath = path_1.default.join(__dirname, '../', 'build', 'Release', 'remote_input.node');
    cModule = require(addonPath);
}
catch {
    // Webpack will fail when just returning the require, so we need to wrap
    // in a try/catch and rethrow.
    /* eslint no-useless-catch: 0 */
    try {
        const addonPath = path_1.default.join(__dirname, '../', 'build', 'Debug', 'remote_input.node');
        cModule = require(addonPath);
    }
    catch (error) {
        throw error;
    }
}
if (!cModule) {
    throw new Error('The native build failed. See the README for help.');
}
const addon = {
    EIOS: cModule.EIOS,
    ImageFormat: Object.freeze({
        BGR_BGRA: ImageFormat.BGR_BGRA,
        BGRA: ImageFormat.BGRA,
        RGBA: ImageFormat.RGBA,
        ARGB: ImageFormat.ARGB,
        ABGR: ImageFormat.ABGR,
    }),
    // Attach JS enums to avoid magic numbers in user code
    // MouseButton mapping follows core (Simba-style): Left=1, Right=0, Middle=2
    MouseButton: Object.freeze({
        Left: MouseButton.Left,
        Right: MouseButton.Right,
        Middle: MouseButton.Middle,
    }),
    // Common Windows Virtual-Key codes for keyboard operations
    // Letters/digits use ASCII codes; include common controls and arrows
    KeyCode: Object.freeze({
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
    })
};
exports.default = addon;
