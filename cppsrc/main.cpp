/* cppsrc/main.cpp */
#include <napi.h>
#include "recordfunc.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return recordfunc::Init(env, exports);
}

NODE_API_MODULE(microphone, InitAll)