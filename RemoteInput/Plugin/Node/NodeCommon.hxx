#pragma once

#include <napi.h>
#include <string>
#include <cstddef>

namespace NodeRI
{
    inline void ThrowNotImplemented(Napi::Env env, const char* api)
    {
        Napi::Error::New(env, std::string(api) + " is not implemented yet").ThrowAsJavaScriptException();
    }

    inline bool EnsureArgCount(const Napi::CallbackInfo& info, std::size_t min, std::size_t max = static_cast<std::size_t>(-1))
    {
        const std::size_t n = info.Length();
        if (n < min || n > max)
        {
            Napi::TypeError::New(info.Env(), "Invalid number of arguments").ThrowAsJavaScriptException();
            return false;
        }
        return true;
    }

    inline bool EnsureType(const Napi::CallbackInfo& info, std::size_t index, napi_valuetype expected)
    {
        if (index >= info.Length())
        {
            Napi::TypeError::New(info.Env(), "Argument index out of range").ThrowAsJavaScriptException();
            return false;
        }
        napi_valuetype actual;
        napi_typeof(info.Env(), info[index], &actual);
        if (actual != expected)
        {
            Napi::TypeError::New(info.Env(), "Invalid argument type").ThrowAsJavaScriptException();
            return false;
        }
        return true;
    }
}
