#pragma once

#include <napi.h>
#include <cstdint>

// Forward declaration to avoid heavy includes
struct EIOS;

class EIOSWrap : public Napi::ObjectWrap<EIOSWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    // JS constructor: new EIOS(pid?: number)
    explicit EIOSWrap(const Napi::CallbackInfo& info);
    ~EIOSWrap();

    // Factory: wrap an existing native EIOS*
    static Napi::Object NewInstance(Napi::Env env, EIOS* native, std::int32_t pid_hint);

private:
    static Napi::FunctionReference constructor;

    // Properties
    Napi::Value GetPid(const Napi::CallbackInfo& info);

    // Static helpers
    static Napi::Value Inject(const Napi::CallbackInfo& info);
    static Napi::Value InjectPid(const Napi::CallbackInfo& info);
    static Napi::Value FromPid(const Napi::CallbackInfo& info);
    static Napi::Value RequestTarget(const Napi::CallbackInfo& info);
    static Napi::Value GetClientsPids(const Napi::CallbackInfo& info);
    static Napi::Value PairClientPid(const Napi::CallbackInfo& info);
    static Napi::Value KillClientPid(const Napi::CallbackInfo& info);

    // Instance methods (Phase 4)
    Napi::Value HasFocus(const Napi::CallbackInfo& info);
    Napi::Value GainFocus(const Napi::CallbackInfo& info);
    Napi::Value LoseFocus(const Napi::CallbackInfo& info);

    Napi::Value IsKeyboardInputEnabled(const Napi::CallbackInfo& info);
    Napi::Value SetKeyboardInputEnabled(const Napi::CallbackInfo& info);
    Napi::Value IsMouseInputEnabled(const Napi::CallbackInfo& info);
    Napi::Value SetMouseInputEnabled(const Napi::CallbackInfo& info);

    Napi::Value GetTargetDimensions(const Napi::CallbackInfo& info);
    Napi::Value GetMousePosition(const Napi::CallbackInfo& info);
    Napi::Value GetRealMousePosition(const Napi::CallbackInfo& info);
    Napi::Value MoveMouse(const Napi::CallbackInfo& info);
    Napi::Value HoldMouse(const Napi::CallbackInfo& info);
    Napi::Value ReleaseMouse(const Napi::CallbackInfo& info);
    Napi::Value ScrollMouse(const Napi::CallbackInfo& info);
    Napi::Value IsMouseButtonHeld(const Napi::CallbackInfo& info);

    Napi::Value SendString(const Napi::CallbackInfo& info);
    Napi::Value HoldKey(const Napi::CallbackInfo& info);
    Napi::Value ReleaseKey(const Napi::CallbackInfo& info);
    Napi::Value IsKeyHeld(const Napi::CallbackInfo& info);
    Napi::Value GetKeyboardSpeed(const Napi::CallbackInfo& info);
    Napi::Value SetKeyboardSpeed(const Napi::CallbackInfo& info);
    Napi::Value GetKeyboardRepeatDelay(const Napi::CallbackInfo& info);
    Napi::Value SetKeyboardRepeatDelay(const Napi::CallbackInfo& info);

    // Graphics (Phase 5)
    Napi::Value GetImageFormat(const Napi::CallbackInfo& info);
    Napi::Value SetImageFormat(const Napi::CallbackInfo& info);
    Napi::Value GetImageBuffer(const Napi::CallbackInfo& info);
    Napi::Value GetDebugImageBuffer(const Napi::CallbackInfo& info);
    Napi::Value SetGraphicsDebugging(const Napi::CallbackInfo& info);

private:
    // Ensure native handle is available; attempts FromPid if missing
    EIOS* EnsureNativeHandle(const Napi::CallbackInfo& info);

    // Keep a separate pid cache for display; native may be null
    std::int32_t pid_ = -1;
    EIOS* native_ = nullptr;
};
