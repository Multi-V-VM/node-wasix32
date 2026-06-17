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
#ifdef __wasi__
  fprintf(stderr,
          "Symbols::Initialize begin target=%p context=%p env=%p\n",
          *target,
          *context,
          static_cast<void*>(env));
  fflush(stderr);
#endif
#define V(PropertyName, StringValue)                                           \
  IF_WASI_SYMBOL_TRACE(PropertyName)                                           \
  target                                                                       \
      ->Set(env->context(),                                                    \
            env->PropertyName()->Description(env->isolate()),                  \
            env->PropertyName())                                               \
      .Check();
#ifdef __wasi__
#define IF_WASI_SYMBOL_TRACE(PropertyName)                                     \
  do {                                                                         \
    Local<v8::Symbol> symbol = env->PropertyName();                            \
    Local<Value> description = symbol->Description(env->isolate());            \
    (void)description;                                                         \
    (void)symbol;                                                              \
    fprintf(stderr,                                                            \
            "Symbols::Initialize set %s symbol=%p description=%p\n",           \
            #PropertyName,                                                     \
            *symbol,                                                           \
            *description);                                                     \
    fflush(stderr);                                                            \
  } while (0);
#else
#define IF_WASI_SYMBOL_TRACE(PropertyName)
#endif
  PER_ISOLATE_SYMBOL_PROPERTIES(V)
#undef V
#undef IF_WASI_SYMBOL_TRACE
#ifdef __wasi__
  fprintf(stderr, "Symbols::Initialize done\n");
  fflush(stderr);
#endif
}

}  // namespace symbols
}  // namespace node

NODE_BINDING_CONTEXT_AWARE_INTERNAL(symbols, node::symbols::Initialize)
