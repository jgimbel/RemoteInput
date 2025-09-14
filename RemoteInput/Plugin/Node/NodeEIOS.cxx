#include "Plugin/Node/NodeEIOS.hxx"
#include "Plugin/Node/NodeCommon.hxx"
#include "EIOS.hxx"
#include "Plugin/NativePlugin.hxx"
#include <cinttypes>

Napi::FunctionReference EIOSWrap::constructor;

EIOSWrap::~EIOSWrap() = default;

Napi::Object EIOSWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function cls = DefineClass(env, "EIOS",
        {
            InstanceAccessor<&EIOSWrap::GetPid>("pid"),
            InstanceMethod("hasFocus", &EIOSWrap::HasFocus),
            InstanceMethod("gainFocus", &EIOSWrap::GainFocus),
            InstanceMethod("loseFocus", &EIOSWrap::LoseFocus),
            InstanceMethod("isKeyboardInputEnabled", &EIOSWrap::IsKeyboardInputEnabled),
            InstanceMethod("setKeyboardInputEnabled", &EIOSWrap::SetKeyboardInputEnabled),
            InstanceMethod("isMouseInputEnabled", &EIOSWrap::IsMouseInputEnabled),
            InstanceMethod("setMouseInputEnabled", &EIOSWrap::SetMouseInputEnabled),
            InstanceMethod("getTargetDimensions", &EIOSWrap::GetTargetDimensions),
            InstanceMethod("getMousePosition", &EIOSWrap::GetMousePosition),
            InstanceMethod("getRealMousePosition", &EIOSWrap::GetRealMousePosition),
            InstanceMethod("moveMouse", &EIOSWrap::MoveMouse),
            InstanceMethod("holdMouse", &EIOSWrap::HoldMouse),
            InstanceMethod("releaseMouse", &EIOSWrap::ReleaseMouse),
            InstanceMethod("scrollMouse", &EIOSWrap::ScrollMouse),
            InstanceMethod("isMouseButtonHeld", &EIOSWrap::IsMouseButtonHeld),
            InstanceMethod("sendString", &EIOSWrap::SendString),
            InstanceMethod("holdKey", &EIOSWrap::HoldKey),
            InstanceMethod("releaseKey", &EIOSWrap::ReleaseKey),
            InstanceMethod("isKeyHeld", &EIOSWrap::IsKeyHeld),
            InstanceMethod("getKeyboardSpeed", &EIOSWrap::GetKeyboardSpeed),
            InstanceMethod("setKeyboardSpeed", &EIOSWrap::SetKeyboardSpeed),
            InstanceMethod("getKeyboardRepeatDelay", &EIOSWrap::GetKeyboardRepeatDelay),
            InstanceMethod("setKeyboardRepeatDelay", &EIOSWrap::SetKeyboardRepeatDelay),
            // Graphics
            InstanceMethod("getImageFormat", &EIOSWrap::GetImageFormat),
            InstanceMethod("setImageFormat", &EIOSWrap::SetImageFormat),
            InstanceMethod("getImageBuffer", &EIOSWrap::GetImageBuffer),
            InstanceMethod("getDebugImageBuffer", &EIOSWrap::GetDebugImageBuffer),
            InstanceMethod("setGraphicsDebugging", &EIOSWrap::SetGraphicsDebugging),
            StaticMethod("inject", &EIOSWrap::Inject),
            StaticMethod("injectPid", &EIOSWrap::InjectPid),
            StaticMethod("fromPid", &EIOSWrap::FromPid),
            StaticMethod("requestTarget", &EIOSWrap::RequestTarget),
            StaticMethod("getClientsPids", &EIOSWrap::GetClientsPids),
            StaticMethod("pairClientPid", &EIOSWrap::PairClientPid),
            StaticMethod("killClientPid", &EIOSWrap::KillClientPid)
        });

    constructor = Napi::Persistent(cls);
    constructor.SuppressDestruct();

    exports.Set("EIOS", cls);
    return exports;
}

EIOSWrap::EIOSWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EIOSWrap>(info)
{
    if (info.Length() >= 1) {
        if (info[0].IsExternal()) {
            auto ext = info[0].As<Napi::External<EIOS>>();
            native_ = ext.Data();
        } else if (info[0].IsNumber()) {
            pid_ = info[0].As<Napi::Number>().Int32Value();
        }
    }
    // Optional second arg as pid hint
    if (info.Length() >= 2 && info[1].IsNumber()) {
        pid_ = info[1].As<Napi::Number>().Int32Value();
    }
}

Napi::Object EIOSWrap::NewInstance(Napi::Env env, EIOS* native, std::int32_t pid_hint)
{
    if (!native) return env.Null().As<Napi::Object>();
    Napi::Object obj = constructor.New({ Napi::External<EIOS>::New(env, native), Napi::Number::New(env, pid_hint) });
    return obj;
}

Napi::Value EIOSWrap::GetPid(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), pid_);
}

// Static helpers
Napi::Value EIOSWrap::Inject(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Undefined();
    std::string name = info[0].As<Napi::String>().Utf8Value();
    EIOS_Inject(name.c_str());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::InjectPid(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS_Inject_PID(pid);
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::FromPid(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Null();
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS* native = EIOS_From_PID(pid);
    if (!native) return info.Env().Null();
    return NewInstance(info.Env(), native, pid);
}

Napi::Value EIOSWrap::RequestTarget(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Null();
    std::string arg = info[0].As<Napi::String>().Utf8Value();
    EIOS* native = EIOS_RequestTarget(arg.c_str());
    if (!native) return info.Env().Null();
    // pid unknown until asked; leave -1
    return NewInstance(info.Env(), native, -1);
}

Napi::Value EIOSWrap::GetClientsPids(const Napi::CallbackInfo& info)
{
    bool unpaired_only = false;
    if (info.Length() >= 1) {
        if (!NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
        unpaired_only = info[0].As<Napi::Boolean>().Value();
    }
    std::size_t count = EIOS_GetClients(unpaired_only);
    Napi::Array arr = Napi::Array::New(info.Env(), count);
    for (std::size_t i = 0; i < count; ++i) {
        arr.Set(i, Napi::Number::New(info.Env(), EIOS_GetClientPID(i)));
    }
    return arr;
}

Napi::Value EIOSWrap::PairClientPid(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Null();
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS* native = EIOS_PairClient(pid);
    if (!native) return info.Env().Null();
    return NewInstance(info.Env(), native, pid);
}

Napi::Value EIOSWrap::KillClientPid(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS_KillClientPID(pid);
    return info.Env().Undefined();
}

// Helper to ensure native handle exists; will try FromPid if missing
EIOS* EIOSWrap::EnsureNativeHandle(const Napi::CallbackInfo& info)
{
    if (!native_ && pid_ >= 0) {
        native_ = EIOS_From_PID(pid_);
    }
    if (!native_) {
        Napi::Error::New(info.Env(), "EIOS native handle is not available. Ensure you used EIOS.fromPid or EIOS.pairClientPid.").ThrowAsJavaScriptException();
        return nullptr;
    }
    return native_;
}

// Helper to ensure a function pointer is available in the DLL
static inline bool ensure_sym(Napi::Env env, const char* name, const void* fn)
{
    if (!fn)
    {
        Napi::Error::New(env, std::string(name) + " not available in core DLL").ThrowAsJavaScriptException();
        return false;
    }
    return true;
}

// Instance methods
Napi::Value EIOSWrap::HasFocus(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), EIOS_HasFocus(h));
}

Napi::Value EIOSWrap::GainFocus(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_GainFocus(h);
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::LoseFocus(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_LoseFocus(h);
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::IsKeyboardInputEnabled(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), EIOS_IsKeyboardInputEnabled(h));
}

Napi::Value EIOSWrap::SetKeyboardInputEnabled(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_SetKeyboardInputEnabled(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::IsMouseInputEnabled(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), EIOS_IsMouseInputEnabled(h));
}

Napi::Value EIOSWrap::SetMouseInputEnabled(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_SetMouseInputEnabled(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::GetTargetDimensions(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::int32_t w = 0, hgt = 0;
    EIOS_GetTargetDimensions(h, &w, &hgt);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("width", Napi::Number::New(info.Env(), w));
    obj.Set("height", Napi::Number::New(info.Env(), hgt));
    return obj;
}

Napi::Value EIOSWrap::GetMousePosition(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::int32_t x = 0, y = 0;
    EIOS_GetMousePosition(h, &x, &y);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), x));
    obj.Set("y", Napi::Number::New(info.Env(), y));
    return obj;
}

Napi::Value EIOSWrap::GetRealMousePosition(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::int32_t x = 0, y = 0;
    EIOS_GetRealMousePosition(h, &x, &y);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), x));
    obj.Set("y", Napi::Number::New(info.Env(), y));
    return obj;
}

Napi::Value EIOSWrap::MoveMouse(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 2, 2) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_MoveMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::HoldMouse(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_HoldMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::ReleaseMouse(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_ReleaseMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::ScrollMouse(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_ScrollMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::IsMouseButtonHeld(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), EIOS_IsMouseButtonHeld(h, info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value EIOSWrap::SendString(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 3) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::string s = info[0].As<Napi::String>().Utf8Value();
    std::int32_t keywait = (info.Length() >= 2 && info[1].IsNumber()) ? info[1].As<Napi::Number>().Int32Value() : 0;
    std::int32_t keymodwait = (info.Length() >= 3 && info[2].IsNumber()) ? info[2].As<Napi::Number>().Int32Value() : 0;
    EIOS_SendString(h, s.c_str(), keywait, keymodwait);
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::HoldKey(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_HoldKey(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::ReleaseKey(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_ReleaseKey(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::IsKeyHeld(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), EIOS_IsKeyHeld(h, info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value EIOSWrap::GetKeyboardSpeed(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Number::New(info.Env(), EIOS_GetKeyboardSpeed(h));
}

Napi::Value EIOSWrap::SetKeyboardSpeed(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_SetKeyboardSpeed(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

Napi::Value EIOSWrap::GetKeyboardRepeatDelay(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    return Napi::Number::New(info.Env(), EIOS_GetKeyboardRepeatDelay(h));
}

Napi::Value EIOSWrap::SetKeyboardRepeatDelay(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_SetKeyboardRepeatDelay(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
}

// MARK: - Graphics
Napi::Value EIOSWrap::GetImageFormat(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    ImageFormat fmt = EIOS_GetImageFormat(h);
    return Napi::Number::New(info.Env(), static_cast<std::uint32_t>(fmt));
}

Napi::Value EIOSWrap::SetImageFormat(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    ImageFormat fmt = static_cast<ImageFormat>(info[0].As<Napi::Number>().Uint32Value());
    EIOS_SetImageFormat(h, fmt);
    return info.Env().Undefined();
}

static inline Napi::Value make_uint8array_from_core(const Napi::CallbackInfo& info, EIOS* h, std::uint8_t* data)
{
    if (!data) return info.Env().Null();
    std::int32_t w = 0, ht = 0;
    EIOS_GetTargetDimensions(h, &w, &ht);
    if (w <= 0 || ht <= 0) return info.Env().Null();
    // All ImageFormat variants are 4 bytes per pixel
    size_t byte_len = static_cast<size_t>(w) * static_cast<size_t>(ht) * 4u;
    auto* ref = new Napi::Reference<Napi::Object>(Napi::Persistent(info.This().As<Napi::Object>()));
    Napi::ArrayBuffer buf = Napi::ArrayBuffer::New(
        info.Env(),
        data,
        byte_len,
        [](Napi::Env, void*, void* hint){
            auto* r = static_cast<Napi::Reference<Napi::Object>*>(hint);
            r->Unref();
            delete r;
        },
        ref
    );
    return Napi::Uint8Array::New(info.Env(), byte_len, buf, 0);
}

Napi::Value EIOSWrap::GetImageBuffer(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::uint8_t* data = EIOS_GetImageBuffer(h);
    return make_uint8array_from_core(info, h, data);
}

Napi::Value EIOSWrap::GetDebugImageBuffer(const Napi::CallbackInfo& info)
{
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    std::uint8_t* data = EIOS_GetDebugImageBuffer(h);
    return make_uint8array_from_core(info, h, data);
}

Napi::Value EIOSWrap::SetGraphicsDebugging(const Napi::CallbackInfo& info)
{
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    EIOS_SetGraphicsDebugging(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
}
