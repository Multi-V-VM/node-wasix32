#include "env-inl.h"
#include "node_binding.h"
#include "util.h"

#ifdef __wasi__
#include <stdio.h>
#endif

namespace node {

using v8::Context;
using v8::Local;
using v8::Object;
using v8::Value;

namespace symbols {

static void Initialize(Local<Object> target,
                       Local<Value> unused,
                       Local<Context> context,
                       void* priv) {
  Environment* env = Environment::GetCurrent(context);
#define V(PropertyName, StringValue)                                           \
  IF_WASI_SYMBOL_TRACE(PropertyName)                                           \
  target                                                                       \
      ->Set(env->context(),                                                    \
            env->PropertyName()->Description(env->isolate()),                  \
            env->PropertyName())                                               \
      .Check();
#define IF_WASI_SYMBOL_TRACE(PropertyName)
  PER_ISOLATE_SYMBOL_PROPERTIES(V)
#undef V
#undef IF_WASI_SYMBOL_TRACE
}

}  // namespace symbols
}  // namespace node

NODE_BINDING_CONTEXT_AWARE_INTERNAL(symbols, node::symbols::Initialize)
