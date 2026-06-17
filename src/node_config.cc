#include "env-inl.h"
#include "memory_tracker.h"
#include "node.h"
#include "node_builtins.h"
#include "node_external_reference.h"
#include "node_i18n.h"
#include "node_options.h"
#include "util-inl.h"
#include <stdio.h>

namespace node {

using v8::Context;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Value;

void GetDefaultLocale(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  std::string locale = isolate->GetDefaultLocale();
  Local<Value> result;
  if (ToV8Value(context, locale).ToLocal(&result)) {
    args.GetReturnValue().Set(result);
  }
}

// The config binding is used to provide an internal view of compile time
// config options that are required internally by lib/*.js code. This is an
// alternative to dropping additional properties onto the process object as
// has been the practice previously in node.cc.

// Command line arguments are already accessible in the JS land via
// require('internal/options').getOptionValue('--some-option'). Do not add them
// here.
static void InitConfig(Local<Object> target,
                       Local<Value> unused,
                       Local<Context> context,
                       void* priv) {
  Environment* env = Environment::GetCurrent(context);
  Isolate* isolate = env->isolate();

#ifdef __wasi__
  fprintf(stderr, "InitConfig: begin\n");
  fflush(stderr);
#endif
#if defined(DEBUG) && DEBUG
  fprintf(stderr, "InitConfig: isDebugBuild true\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "isDebugBuild");
#else
  fprintf(stderr, "InitConfig: isDebugBuild false\n");
  fflush(stderr);
  READONLY_FALSE_PROPERTY(target, "isDebugBuild");
#endif  // defined(DEBUG) && DEBUG

#ifdef OPENSSL_IS_BORINGSSL
  fprintf(stderr, "InitConfig: openSSLIsBoringSSL true\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "openSSLIsBoringSSL");
#else
  fprintf(stderr, "InitConfig: openSSLIsBoringSSL false\n");
  fflush(stderr);
  READONLY_FALSE_PROPERTY(target, "openSSLIsBoringSSL");
#endif  // OPENSSL_IS_BORINGSSL

#if HAVE_OPENSSL
  fprintf(stderr, "InitConfig: hasOpenSSL true\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasOpenSSL");
#else
  fprintf(stderr, "InitConfig: hasOpenSSL false\n");
  fflush(stderr);
  READONLY_FALSE_PROPERTY(target, "hasOpenSSL");
#endif  // HAVE_OPENSSL

  fprintf(stderr, "InitConfig: fipsMode\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "fipsMode");

#ifdef NODE_HAVE_I18N_SUPPORT

  fprintf(stderr, "InitConfig: hasIntl\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasIntl");

#ifdef NODE_HAVE_SMALL_ICU
  fprintf(stderr, "InitConfig: hasSmallICU\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasSmallICU");
#endif  // NODE_HAVE_SMALL_ICU

#if NODE_USE_V8_PLATFORM
  fprintf(stderr, "InitConfig: hasTracing\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasTracing");
#endif

#if !defined(NODE_WITHOUT_NODE_OPTIONS)
  fprintf(stderr, "InitConfig: hasNodeOptions\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasNodeOptions");
#endif

#endif  // NODE_HAVE_I18N_SUPPORT

#if HAVE_INSPECTOR
  fprintf(stderr, "InitConfig: hasInspector true\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "hasInspector");
#else
  fprintf(stderr, "InitConfig: hasInspector false\n");
  fflush(stderr);
  READONLY_FALSE_PROPERTY(target, "hasInspector");
#endif

// configure --no-browser-globals
#ifdef NODE_NO_BROWSER_GLOBALS
  fprintf(stderr, "InitConfig: noBrowserGlobals true\n");
  fflush(stderr);
  READONLY_TRUE_PROPERTY(target, "noBrowserGlobals");
#else
  fprintf(stderr, "InitConfig: noBrowserGlobals false\n");
  fflush(stderr);
  READONLY_FALSE_PROPERTY(target, "noBrowserGlobals");
#endif  // NODE_NO_BROWSER_GLOBALS

  fprintf(stderr, "InitConfig: bits\n");
  fflush(stderr);
  READONLY_PROPERTY(target, "bits", Number::New(isolate, 8 * sizeof(intptr_t)));

  fprintf(stderr, "InitConfig: getDefaultLocale\n");
  fflush(stderr);
  SetMethodNoSideEffect(context, target, "getDefaultLocale", GetDefaultLocale);
  fprintf(stderr, "InitConfig: done\n");
  fflush(stderr);
}  // InitConfig

void RegisterConfigExternalReferences(ExternalReferenceRegistry* registry) {
  registry->Register(GetDefaultLocale);
}

}  // namespace node

NODE_BINDING_CONTEXT_AWARE_INTERNAL(config, node::InitConfig)
NODE_BINDING_EXTERNAL_REFERENCE(config, node::RegisterConfigExternalReferences)
