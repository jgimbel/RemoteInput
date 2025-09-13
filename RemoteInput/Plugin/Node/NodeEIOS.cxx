#include "Plugin/Node/NodeEIOS.hxx"
#include "Plugin/Node/NodeCommon.hxx"
#include <cinttypes>

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

Napi::FunctionReference EIOSWrap::constructor;

// Minimal native API surface (opaque EIOS and functions), loaded dynamically on Windows
namespace {
#if defined(_WIN32) || defined(_WIN64)
    // stdcall matches EIOS exports on Windows
    typedef void (__stdcall *FnEIOS_Inject)(const char*);
    typedef void (__stdcall *FnEIOS_Inject_PID)(std::int32_t);
    typedef EIOS* (__stdcall *FnEIOS_From_PID)(std::int32_t);
    typedef EIOS* (__stdcall *FnEIOS_RequestTarget)(const char*);
    typedef std::size_t (__stdcall *FnEIOS_GetClients)(bool);
    typedef std::int32_t (__stdcall *FnEIOS_GetClientPID)(std::size_t);
    typedef EIOS* (__stdcall *FnEIOS_PairClient)(std::int32_t);
    typedef void (__stdcall *FnEIOS_KillClientPID)(std::int32_t);
    typedef void (__stdcall *FnEIOS_ReleaseTarget)(EIOS*);

    // Instance API
    typedef bool (__stdcall *FnEIOS_HasFocus)(EIOS*);
    typedef void (__stdcall *FnEIOS_GainFocus)(EIOS*);
    typedef void (__stdcall *FnEIOS_LoseFocus)(EIOS*);
    typedef bool (__stdcall *FnEIOS_IsKeyboardInputEnabled)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetKeyboardInputEnabled)(EIOS*, bool);
    typedef bool (__stdcall *FnEIOS_IsMouseInputEnabled)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetMouseInputEnabled)(EIOS*, bool);
    typedef void (__stdcall *FnEIOS_GetTargetDimensions)(EIOS*, std::int32_t*, std::int32_t*);
    typedef void (__stdcall *FnEIOS_GetMousePosition)(EIOS*, std::int32_t*, std::int32_t*);
    typedef void (__stdcall *FnEIOS_GetRealMousePosition)(EIOS*, std::int32_t*, std::int32_t*);
    typedef void (__stdcall *FnEIOS_MoveMouse)(EIOS*, std::int32_t, std::int32_t);
    typedef void (__stdcall *FnEIOS_HoldMouse)(EIOS*, std::int32_t, std::int32_t, std::int32_t);
    typedef void (__stdcall *FnEIOS_ReleaseMouse)(EIOS*, std::int32_t, std::int32_t, std::int32_t);
    typedef void (__stdcall *FnEIOS_ScrollMouse)(EIOS*, std::int32_t, std::int32_t, std::int32_t);
    typedef bool (__stdcall *FnEIOS_IsMouseButtonHeld)(EIOS*, std::int32_t);
    typedef void (__stdcall *FnEIOS_SendString)(EIOS*, const char*, std::int32_t, std::int32_t);
    typedef void (__stdcall *FnEIOS_HoldKey)(EIOS*, std::int32_t);
    typedef void (__stdcall *FnEIOS_ReleaseKey)(EIOS*, std::int32_t);
    typedef bool (__stdcall *FnEIOS_IsKeyHeld)(EIOS*, std::int32_t);
    typedef std::int32_t (__stdcall *FnEIOS_GetKeyboardSpeed)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetKeyboardSpeed)(EIOS*, std::int32_t);
    typedef std::int32_t (__stdcall *FnEIOS_GetKeyboardRepeatDelay)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetKeyboardRepeatDelay)(EIOS*, std::int32_t);

    // Graphics API
    typedef std::uint32_t (__stdcall *FnEIOS_GetImageFormat)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetImageFormat)(EIOS*, std::uint32_t);
    typedef std::uint8_t* (__stdcall *FnEIOS_GetImageBuffer)(EIOS*);
    typedef std::uint8_t* (__stdcall *FnEIOS_GetDebugImageBuffer)(EIOS*);
    typedef void (__stdcall *FnEIOS_SetGraphicsDebugging)(EIOS*, bool);

    struct CoreAPI {
        HMODULE module { nullptr };
        FnEIOS_Inject Inject { nullptr };
        FnEIOS_Inject_PID InjectPid { nullptr };
        FnEIOS_From_PID FromPid { nullptr };
        FnEIOS_RequestTarget RequestTarget { nullptr };
        FnEIOS_GetClients GetClients { nullptr };
        FnEIOS_GetClientPID GetClientPID { nullptr };
        FnEIOS_PairClient PairClient { nullptr };
        FnEIOS_KillClientPID KillClientPid { nullptr };
        FnEIOS_ReleaseTarget ReleaseTarget { nullptr };
        // Instance API
        FnEIOS_HasFocus HasFocus { nullptr };
        FnEIOS_GainFocus GainFocus { nullptr };
        FnEIOS_LoseFocus LoseFocus { nullptr };
        FnEIOS_IsKeyboardInputEnabled IsKeyboardInputEnabled { nullptr };
        FnEIOS_SetKeyboardInputEnabled SetKeyboardInputEnabled { nullptr };
        FnEIOS_IsMouseInputEnabled IsMouseInputEnabled { nullptr };
        FnEIOS_SetMouseInputEnabled SetMouseInputEnabled { nullptr };
        FnEIOS_GetTargetDimensions GetTargetDimensions { nullptr };
        FnEIOS_GetMousePosition GetMousePosition { nullptr };
        FnEIOS_GetRealMousePosition GetRealMousePosition { nullptr };
        FnEIOS_MoveMouse MoveMouse { nullptr };
        FnEIOS_HoldMouse HoldMouse { nullptr };
        FnEIOS_ReleaseMouse ReleaseMouse { nullptr };
        FnEIOS_ScrollMouse ScrollMouse { nullptr };
        FnEIOS_IsMouseButtonHeld IsMouseButtonHeld { nullptr };
        FnEIOS_SendString SendString { nullptr };
        FnEIOS_HoldKey HoldKey { nullptr };
        FnEIOS_ReleaseKey ReleaseKey { nullptr };
        FnEIOS_IsKeyHeld IsKeyHeld { nullptr };
        FnEIOS_GetKeyboardSpeed GetKeyboardSpeed { nullptr };
        FnEIOS_SetKeyboardSpeed SetKeyboardSpeed { nullptr };
        FnEIOS_GetKeyboardRepeatDelay GetKeyboardRepeatDelay { nullptr };
        FnEIOS_SetKeyboardRepeatDelay SetKeyboardRepeatDelay { nullptr };

        // Graphics API
        FnEIOS_GetImageFormat GetImageFormat { nullptr };
        FnEIOS_SetImageFormat SetImageFormat { nullptr };
        FnEIOS_GetImageBuffer GetImageBuffer { nullptr };
        FnEIOS_GetDebugImageBuffer GetDebugImageBuffer { nullptr };
        FnEIOS_SetGraphicsDebugging SetGraphicsDebugging { nullptr };
    } g_api;

    FARPROC load_sym(const char* name) {
        return g_api.module ? GetProcAddress(g_api.module, name) : nullptr;
    }

    bool ensure_core_loaded() {
        if (g_api.module)
            return true;

        // Try environment override first
        char path[MAX_PATH] = {0};
        DWORD len = GetEnvironmentVariableA("REMOTE_INPUT_DLL", path, MAX_PATH);
        if (len > 0 && len < MAX_PATH) {
            g_api.module = LoadLibraryA(path);
        }
        // Try default DLL names if not set
        
        #if defined(_WIN32)
        if (!g_api.module) {
            g_api.module = LoadLibraryA("libRemoteInput-i686.dll");
        }
        #elif defined(_WIN64)
        if (!g_api.module) {
            g_api.module = LoadLibraryA("libRemoteInput-x86_64.dll");
        }
        #elif defined(__APPLE__)
        if (!g_api.module) {
            g_api.module = dlopen("libRemoteInput-x86_64.dylib", RTLD_LAZY);
        }
        #elif defined(__aarch64__) || defined(__arm__)
        if (!g_api.module) {
            g_api.module = dlopen("libRemoteInput-aarch64.so", RTLD_LAZY);
        }
        #elif defined(__linux__) && (defined(__x86_64__) || defined(__i386__))
        if (!g_api.module) {
            g_api.module = dlopen("libRemoteInput-x86_64.so", RTLD_LAZY);
        }
        #endif
        if (!g_api.module) {
            return false;
        }

        g_api.Inject        = reinterpret_cast<FnEIOS_Inject>(load_sym("EIOS_Inject"));
        g_api.InjectPid     = reinterpret_cast<FnEIOS_Inject_PID>(load_sym("EIOS_Inject_PID"));
        g_api.FromPid       = reinterpret_cast<FnEIOS_From_PID>(load_sym("EIOS_From_PID"));
        g_api.RequestTarget = reinterpret_cast<FnEIOS_RequestTarget>(load_sym("EIOS_RequestTarget"));
        g_api.GetClients    = reinterpret_cast<FnEIOS_GetClients>(load_sym("EIOS_GetClients"));
        g_api.GetClientPID  = reinterpret_cast<FnEIOS_GetClientPID>(load_sym("EIOS_GetClientPID"));
        g_api.PairClient    = reinterpret_cast<FnEIOS_PairClient>(load_sym("EIOS_PairClient"));
        g_api.KillClientPid = reinterpret_cast<FnEIOS_KillClientPID>(load_sym("EIOS_KillClientPID"));
        g_api.ReleaseTarget = reinterpret_cast<FnEIOS_ReleaseTarget>(load_sym("EIOS_ReleaseTarget"));

        // Instance API
        g_api.HasFocus = reinterpret_cast<FnEIOS_HasFocus>(load_sym("EIOS_HasFocus"));
        g_api.GainFocus = reinterpret_cast<FnEIOS_GainFocus>(load_sym("EIOS_GainFocus"));
        g_api.LoseFocus = reinterpret_cast<FnEIOS_LoseFocus>(load_sym("EIOS_LoseFocus"));
        g_api.IsKeyboardInputEnabled = reinterpret_cast<FnEIOS_IsKeyboardInputEnabled>(load_sym("EIOS_IsKeyboardInputEnabled"));
        g_api.SetKeyboardInputEnabled = reinterpret_cast<FnEIOS_SetKeyboardInputEnabled>(load_sym("EIOS_SetKeyboardInputEnabled"));
        g_api.IsMouseInputEnabled = reinterpret_cast<FnEIOS_IsMouseInputEnabled>(load_sym("EIOS_IsMouseInputEnabled"));
        g_api.SetMouseInputEnabled = reinterpret_cast<FnEIOS_SetMouseInputEnabled>(load_sym("EIOS_SetMouseInputEnabled"));
        g_api.GetTargetDimensions = reinterpret_cast<FnEIOS_GetTargetDimensions>(load_sym("EIOS_GetTargetDimensions"));
        g_api.GetMousePosition = reinterpret_cast<FnEIOS_GetMousePosition>(load_sym("EIOS_GetMousePosition"));
        g_api.GetRealMousePosition = reinterpret_cast<FnEIOS_GetRealMousePosition>(load_sym("EIOS_GetRealMousePosition"));
        g_api.MoveMouse = reinterpret_cast<FnEIOS_MoveMouse>(load_sym("EIOS_MoveMouse"));
        g_api.HoldMouse = reinterpret_cast<FnEIOS_HoldMouse>(load_sym("EIOS_HoldMouse"));
        g_api.ReleaseMouse = reinterpret_cast<FnEIOS_ReleaseMouse>(load_sym("EIOS_ReleaseMouse"));
        g_api.ScrollMouse = reinterpret_cast<FnEIOS_ScrollMouse>(load_sym("EIOS_ScrollMouse"));
        g_api.IsMouseButtonHeld = reinterpret_cast<FnEIOS_IsMouseButtonHeld>(load_sym("EIOS_IsMouseButtonHeld"));
        if (!g_api.IsMouseButtonHeld) {
            // Fallback for older builds exporting EIOS_IsMouseHeld
            g_api.IsMouseButtonHeld = reinterpret_cast<FnEIOS_IsMouseButtonHeld>(load_sym("EIOS_IsMouseHeld"));
        }
        g_api.SendString = reinterpret_cast<FnEIOS_SendString>(load_sym("EIOS_SendString"));
        g_api.HoldKey = reinterpret_cast<FnEIOS_HoldKey>(load_sym("EIOS_HoldKey"));
        g_api.ReleaseKey = reinterpret_cast<FnEIOS_ReleaseKey>(load_sym("EIOS_ReleaseKey"));
        g_api.IsKeyHeld = reinterpret_cast<FnEIOS_IsKeyHeld>(load_sym("EIOS_IsKeyHeld"));
        g_api.GetKeyboardSpeed = reinterpret_cast<FnEIOS_GetKeyboardSpeed>(load_sym("EIOS_GetKeyboardSpeed"));
        g_api.SetKeyboardSpeed = reinterpret_cast<FnEIOS_SetKeyboardSpeed>(load_sym("EIOS_SetKeyboardSpeed"));
        g_api.GetKeyboardRepeatDelay = reinterpret_cast<FnEIOS_GetKeyboardRepeatDelay>(load_sym("EIOS_GetKeyboardRepeatDelay"));
        g_api.SetKeyboardRepeatDelay = reinterpret_cast<FnEIOS_SetKeyboardRepeatDelay>(load_sym("EIOS_SetKeyboardRepeatDelay"));

        // Graphics API
        g_api.GetImageFormat = reinterpret_cast<FnEIOS_GetImageFormat>(load_sym("EIOS_GetImageFormat"));
        g_api.SetImageFormat = reinterpret_cast<FnEIOS_SetImageFormat>(load_sym("EIOS_SetImageFormat"));
        g_api.GetImageBuffer = reinterpret_cast<FnEIOS_GetImageBuffer>(load_sym("EIOS_GetImageBuffer"));
        g_api.GetDebugImageBuffer = reinterpret_cast<FnEIOS_GetDebugImageBuffer>(load_sym("EIOS_GetDebugImageBuffer"));
        g_api.SetGraphicsDebugging = reinterpret_cast<FnEIOS_SetGraphicsDebugging>(load_sym("EIOS_SetGraphicsDebugging"));

        return g_api.Inject && g_api.InjectPid && g_api.FromPid && g_api.RequestTarget &&
               g_api.GetClients && g_api.GetClientPID && g_api.PairClient && g_api.KillClientPid && g_api.ReleaseTarget;
    }
#else
    bool ensure_core_loaded() { return false; }
#endif
} // namespace

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
    // Export ImageFormat enum (mirrors native order)
    Napi::Object imageFormat = Napi::Object::New(env);
    imageFormat.Set("BGR_BGRA", Napi::Number::New(env, 0));
    imageFormat.Set("BGRA", Napi::Number::New(env, 1));
    imageFormat.Set("RGBA", Napi::Number::New(env, 2));
    imageFormat.Set("ARGB", Napi::Number::New(env, 3));
    imageFormat.Set("ABGR", Napi::Number::New(env, 4));
    exports.Set("ImageFormat", imageFormat);
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
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Undefined();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::string name = info[0].As<Napi::String>().Utf8Value();
    g_api.Inject(name.c_str());
    return info.Env().Undefined();
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.inject");
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::InjectPid(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    g_api.InjectPid(pid);
    return info.Env().Undefined();
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.injectPid");
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::FromPid(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Null();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Null(); }
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS* native = g_api.FromPid(pid);
    if (!native) return info.Env().Null();
    return NewInstance(info.Env(), native, pid);
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.fromPid");
    return info.Env().Null();
#endif
}

Napi::Value EIOSWrap::RequestTarget(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Null();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Null(); }
    std::string arg = info[0].As<Napi::String>().Utf8Value();
    EIOS* native = g_api.RequestTarget(arg.c_str());
    if (!native) return info.Env().Null();
    // pid unknown until asked; leave -1
    return NewInstance(info.Env(), native, -1);
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.requestTarget");
    return info.Env().Null();
#endif
}

Napi::Value EIOSWrap::GetClientsPids(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    bool unpaired_only = false;
    if (info.Length() >= 1) {
        if (!NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
        unpaired_only = info[0].As<Napi::Boolean>().Value();
    }
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::size_t count = g_api.GetClients(unpaired_only);
    Napi::Array arr = Napi::Array::New(info.Env(), count);
    for (std::size_t i = 0; i < count; ++i) {
        arr.Set(i, Napi::Number::New(info.Env(), g_api.GetClientPID(i)));
    }
    return arr;
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.getClientsPids");
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::PairClientPid(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Null();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Null(); }
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    EIOS* native = g_api.PairClient(pid);
    if (!native) return info.Env().Null();
    return NewInstance(info.Env(), native, pid);
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.pairClientPid");
    return info.Env().Null();
#endif
}

Napi::Value EIOSWrap::KillClientPid(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    if (!ensure_core_loaded()) { Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::int32_t pid = info[0].As<Napi::Number>().Int32Value();
    g_api.KillClientPid(pid);
    return info.Env().Undefined();
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS.killClientPid");
    return info.Env().Undefined();
#endif
}

// Helper to ensure native handle exists; will try FromPid if missing
EIOS* EIOSWrap::EnsureNativeHandle(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!ensure_core_loaded()) {
        Napi::Error::New(info.Env(), "libRemoteInput.dll not found. Set REMOTE_INPUT_DLL to its path or add it to PATH.").ThrowAsJavaScriptException();
        return nullptr;
    }
    if (!native_ && pid_ >= 0 && g_api.FromPid) {
        native_ = g_api.FromPid(pid_);
    }
    if (!native_) {
        Napi::Error::New(info.Env(), "EIOS native handle is not available. Ensure you used EIOS.fromPid or EIOS.pairClientPid.").ThrowAsJavaScriptException();
        return nullptr;
    }
    return native_;
#else
    NodeRI::ThrowNotImplemented(info.Env(), "EIOS instance methods");
    return nullptr;
#endif
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
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_HasFocus", g_api.HasFocus)) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), g_api.HasFocus(h));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GainFocus(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GainFocus", g_api.GainFocus)) return info.Env().Undefined();
    g_api.GainFocus(h);
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::LoseFocus(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_LoseFocus", g_api.LoseFocus)) return info.Env().Undefined();
    g_api.LoseFocus(h);
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::IsKeyboardInputEnabled(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_IsKeyboardInputEnabled", g_api.IsKeyboardInputEnabled)) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), g_api.IsKeyboardInputEnabled(h));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetKeyboardInputEnabled(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_SetKeyboardInputEnabled", g_api.SetKeyboardInputEnabled)) return info.Env().Undefined();
    g_api.SetKeyboardInputEnabled(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::IsMouseInputEnabled(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_IsMouseInputEnabled", g_api.IsMouseInputEnabled)) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), g_api.IsMouseInputEnabled(h));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetMouseInputEnabled(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_SetMouseInputEnabled", g_api.SetMouseInputEnabled)) return info.Env().Undefined();
    g_api.SetMouseInputEnabled(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetTargetDimensions(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GetTargetDimensions", g_api.GetTargetDimensions)) return info.Env().Undefined();
    std::int32_t w = 0, hgt = 0;
    g_api.GetTargetDimensions(h, &w, &hgt);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("width", Napi::Number::New(info.Env(), w));
    obj.Set("height", Napi::Number::New(info.Env(), hgt));
    return obj;
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetMousePosition(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GetMousePosition", g_api.GetMousePosition)) return info.Env().Undefined();
    std::int32_t x = 0, y = 0;
    g_api.GetMousePosition(h, &x, &y);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), x));
    obj.Set("y", Napi::Number::New(info.Env(), y));
    return obj;
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetRealMousePosition(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GetRealMousePosition", g_api.GetRealMousePosition)) return info.Env().Undefined();
    std::int32_t x = 0, y = 0;
    g_api.GetRealMousePosition(h, &x, &y);
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), x));
    obj.Set("y", Napi::Number::New(info.Env(), y));
    return obj;
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::MoveMouse(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 2, 2) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_MoveMouse", g_api.MoveMouse)) return info.Env().Undefined();
    g_api.MoveMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::HoldMouse(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_HoldMouse", g_api.HoldMouse)) return info.Env().Undefined();
    g_api.HoldMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::ReleaseMouse(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_ReleaseMouse", g_api.ReleaseMouse)) return info.Env().Undefined();
    g_api.ReleaseMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::ScrollMouse(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 3, 3) || !NodeRI::EnsureType(info, 0, napi_number) || !NodeRI::EnsureType(info, 1, napi_number) || !NodeRI::EnsureType(info, 2, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_ScrollMouse", g_api.ScrollMouse)) return info.Env().Undefined();
    g_api.ScrollMouse(h, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::IsMouseButtonHeld(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_IsMouseButtonHeld", g_api.IsMouseButtonHeld)) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), g_api.IsMouseButtonHeld(h, info[0].As<Napi::Number>().Int32Value()));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SendString(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 3) || !NodeRI::EnsureType(info, 0, napi_string)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_SendString", g_api.SendString)) return info.Env().Undefined();
    std::string s = info[0].As<Napi::String>().Utf8Value();
    std::int32_t keywait = (info.Length() >= 2 && info[1].IsNumber()) ? info[1].As<Napi::Number>().Int32Value() : 0;
    std::int32_t keymodwait = (info.Length() >= 3 && info[2].IsNumber()) ? info[2].As<Napi::Number>().Int32Value() : 0;
    g_api.SendString(h, s.c_str(), keywait, keymodwait);
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::HoldKey(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_HoldKey", g_api.HoldKey)) return info.Env().Undefined();
    g_api.HoldKey(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::ReleaseKey(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_ReleaseKey", g_api.ReleaseKey)) return info.Env().Undefined();
    g_api.ReleaseKey(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::IsKeyHeld(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_IsKeyHeld", g_api.IsKeyHeld)) return info.Env().Undefined();
    return Napi::Boolean::New(info.Env(), g_api.IsKeyHeld(h, info[0].As<Napi::Number>().Int32Value()));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetKeyboardSpeed(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GetKeyboardSpeed", g_api.GetKeyboardSpeed)) return info.Env().Undefined();
    return Napi::Number::New(info.Env(), g_api.GetKeyboardSpeed(h));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetKeyboardSpeed(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_SetKeyboardSpeed", g_api.SetKeyboardSpeed)) return info.Env().Undefined();
    g_api.SetKeyboardSpeed(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetKeyboardRepeatDelay(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_GetKeyboardRepeatDelay", g_api.GetKeyboardRepeatDelay)) return info.Env().Undefined();
    return Napi::Number::New(info.Env(), g_api.GetKeyboardRepeatDelay(h));
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetKeyboardRepeatDelay(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!ensure_sym(info.Env(), "EIOS_SetKeyboardRepeatDelay", g_api.SetKeyboardRepeatDelay)) return info.Env().Undefined();
    g_api.SetKeyboardRepeatDelay(h, info[0].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

// MARK: - Graphics
Napi::Value EIOSWrap::GetImageFormat(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!g_api.GetImageFormat) { Napi::Error::New(info.Env(), "EIOS_GetImageFormat not available in core DLL").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::uint32_t fmt = g_api.GetImageFormat(h);
    return Napi::Number::New(info.Env(), fmt);
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetImageFormat(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_number)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!g_api.SetImageFormat) { Napi::Error::New(info.Env(), "EIOS_SetImageFormat not available in core DLL").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::uint32_t fmt = info[0].As<Napi::Number>().Uint32Value();
    g_api.SetImageFormat(h, fmt);
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}

static inline Napi::Value make_uint8array_from_core(const Napi::CallbackInfo& info, EIOS* h, std::uint8_t* data)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!data) return info.Env().Null();
    std::int32_t w = 0, ht = 0;
    g_api.GetTargetDimensions(h, &w, &ht);
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
#else
    return info.Env().Null();
#endif
}

Napi::Value EIOSWrap::GetImageBuffer(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!g_api.GetImageBuffer) { Napi::Error::New(info.Env(), "EIOS_GetImageBuffer not available in core DLL").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::uint8_t* data = g_api.GetImageBuffer(h);
    return make_uint8array_from_core(info, h, data);
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::GetDebugImageBuffer(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!g_api.GetDebugImageBuffer) { Napi::Error::New(info.Env(), "EIOS_GetDebugImageBuffer not available in core DLL").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    std::uint8_t* data = g_api.GetDebugImageBuffer(h);
    return make_uint8array_from_core(info, h, data);
#else
    return info.Env().Undefined();
#endif
}

Napi::Value EIOSWrap::SetGraphicsDebugging(const Napi::CallbackInfo& info)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!NodeRI::EnsureArgCount(info, 1, 1) || !NodeRI::EnsureType(info, 0, napi_boolean)) return info.Env().Undefined();
    EIOS* h = EnsureNativeHandle(info); if (!h) return info.Env().Undefined();
    if (!g_api.SetGraphicsDebugging) { Napi::Error::New(info.Env(), "EIOS_SetGraphicsDebugging not available in core DLL").ThrowAsJavaScriptException(); return info.Env().Undefined(); }
    g_api.SetGraphicsDebugging(h, info[0].As<Napi::Boolean>().Value());
    return info.Env().Undefined();
#else
    return info.Env().Undefined();
#endif
}
