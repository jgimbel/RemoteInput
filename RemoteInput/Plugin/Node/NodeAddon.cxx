#include <napi.h>
#include "Plugin/Node/NodeEIOS.hxx"

static Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return EIOSWrap::Init(env, exports);
}

NODE_API_MODULE(remote_input, InitAll)
