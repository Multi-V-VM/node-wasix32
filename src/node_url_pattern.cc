#include "node_url_pattern.h"
#include "base_object-inl.h"
#include "debug_utils-inl.h"
#include "env-inl.h"
#include "node.h"
#include "node_errors.h"
#include "node_mem-inl.h"
#include "path.h"
#include "util-inl.h"

namespace node {
using node::url_pattern::URLPatternRegexProvider;

// Forward declaration of MemoryRetainerTraits template
template <typename T>
struct MemoryRetainerTraits;

template <>
struct MemoryRetainerTraits<ada::url_pattern<URLPatternRegexProvider>> {
  using Type = ada::url_pattern<URLPatternRegexProvider>;
  static void MemoryInfo(MemoryTracker* tracker, const Type& value) {
    // WASI stub - no memory tracking implementation
    (void)tracker;
    (void)value;
  }

  static const char* MemoryInfoName(const Type& value) {
    return "ada::url_pattern";
  }

  static size_t SelfSize(const Type& value) { return sizeof(value); }
};

// WASI stub - removed duplicate MemoryRetainerTraits

}  // namespace node

namespace node::url_pattern {

using v8::Array;
using v8::Context;
using v8::DontDelete;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Global;
using v8::Isolate;
using v8::Local;
using v8::LocalVector;
using v8::MaybeLocal;
using v8::Name;
using v8::NewStringType;
using v8::Object;
using v8::PropertyAttribute;
using v8::ReadOnly;
using v8::RegExp;
using v8::Signature;
using v8::String;
using v8::Value;

std::optional<URLPatternRegexProvider::regex_type>
URLPatternRegexProvider::create_instance(std::string_view pattern,
                                         bool ignore_case) {
  auto isolate = Isolate::GetCurrent();
  auto env = Environment::GetCurrent(isolate);
  int flags = RegExp::Flags::kUnicodeSets | RegExp::Flags::kDotAll;
  if (ignore_case) {
    flags |= static_cast<int>(RegExp::Flags::kIgnoreCase);
  }

  Local<String> local_pattern;
  if (!String::NewFromUtf8(
           isolate, pattern.data(), NewStringType::kNormal, pattern.size())
           .ToLocal(&local_pattern)) {
    return std::nullopt;
  }
  Local<RegExp> regexp;
  if (!RegExp::New(
           env->context(), local_pattern, static_cast<RegExp::Flags>(flags))
           .ToLocal(&regexp)) {
    return std::nullopt;
  }
  return Global<RegExp>(isolate, regexp);
}

bool URLPatternRegexProvider::regex_match(std::string_view input,
                                          const regex_type& pattern) {
  auto isolate = Isolate::GetCurrent();
  auto env = Environment::GetCurrent(isolate);
  Local<String> local_input;
  if (!String::NewFromUtf8(
           isolate, input.data(), NewStringType::kNormal, input.size())
           .ToLocal(&local_input)) {
    return false;
  }
  Local<Object> result_object;
  if (!pattern.Get(isolate)
           ->Exec(env->context(), local_input)
           .ToLocal(&result_object)) {
    return false;
  }
  // RegExp::Exec returns null if there is no match.
  return !result_object->IsNull();
}

std::optional<std::vector<std::optional<std::string>>>
URLPatternRegexProvider::regex_search(std::string_view input,
                                      const regex_type& global_pattern) {
  auto isolate = Isolate::GetCurrent();
  auto env = Environment::GetCurrent(isolate);
  Local<String> local_input;
  if (!String::NewFromUtf8(
           isolate, input.data(), NewStringType::kNormal, input.size())
           .ToLocal(&local_input)) {
    return std::nullopt;
  }
  Local<Object> exec_result_object;
  auto pattern = global_pattern.Get(isolate);
  if (!pattern->Exec(env->context(), local_input)
           .ToLocal(&exec_result_object) ||
      exec_result_object->IsNull()) {
    return std::nullopt;
  }
  DCHECK(exec_result_object->IsArray());
  auto exec_result = exec_result_object.As<Array>();
  size_t len = exec_result->Length();
  std::vector<std::optional<std::string>> result;
  result.reserve(len);
  for (size_t i = 1; i < len; i++) {
    Local<Value> entry;
    if (!exec_result->Get(env->context(), i).ToLocal(&entry)) {
      return std::nullopt;
    }

    // V8 checks that the regexp exec result is one of the correct types.
    DCHECK_IMPLIES(!entry->IsUndefined(), entry->IsString());

    if (entry->IsUndefined()) {
      result.emplace_back(std::nullopt);
    } else if (entry->IsString()) {
      Utf8Value utf8_entry(isolate, entry.As<String>());
      result.emplace_back(utf8_entry.ToString());
    }
  }
  return result;
}

URLPattern::URLPattern(Environment* env,
                       Local<Object> object,
                       ada::url_pattern<URLPatternRegexProvider>&& url_pattern)
    : BaseObject(env, object), url_pattern_(std::move(url_pattern)) {
  MakeWeak();
}

void URLPattern::MemoryInfo(MemoryTracker* tracker) const {
  tracker->TrackField("url_pattern", url_pattern_);
#define URL_PATTERN_CACHED_VALUES(_, lowercase)                                \
  tracker->TrackField(#lowercase, lowercase);
  URL_PATTERN_COMPONENTS(URL_PATTERN_CACHED_VALUES)
#undef URL_PATTERN_CACHED_VALUES
}

void URLPattern::New(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);

  if (!args.IsConstructCall()) {
    THROW_ERR_CONSTRUCT_CALL_REQUIRED(env);
    return;
  }

  // If no arguments are passed, then we parse the empty URLPattern.
  if (args.Length() == 0) {
    auto init = ada::url_pattern_init{};
    auto url_pattern = ada::parse_url_pattern<URLPatternRegexProvider>(init);
    CHECK(url_pattern);
    new URLPattern(env, args.This(), std::move(*url_pattern));
    return;
  }

  std::optional<ada::url_pattern_init> init{};
  std::optional<std::string> input{};
  std::optional<std::string> base_url{};
  std::optional<ada::url_pattern_options> options{};

  // Following patterns are supported:
  // - new URLPattern(input)
  // - new URLPattern(input, baseURL)
  // - new URLPattern(input, options)
  // - new URLPattern(input, baseURL, options)
  if (args[0]->IsString()) {
    BufferValue input_buffer(env->isolate(), args[0]);
    CHECK_NOT_NULL(*input_buffer);
    input = input_buffer.ToString();
  } else if (args[0]->IsObject()) {
    init = URLPatternInit::FromJsObject(env, args[0].As<Object>());
    // If init does not have a value here, the implication is that an
    // error was thrown. Let's allow that to be handled now by returning
    // early. If we don't, the error thrown will be swallowed.
    if (!init) return;
  } else {
    THROW_ERR_INVALID_ARG_TYPE(env, "Input must be an object or a string");
    return;
  }

  // The next argument can be baseURL or options.
  if (args.Length() > 1) {
    if (args[1]->IsString()) {
      BufferValue base_url_buffer(env->isolate(), args[1]);
      CHECK_NOT_NULL(*base_url_buffer);
      base_url = base_url_buffer.ToString();
    } else if (args[1]->IsObject()) {
      CHECK(!options.has_value());
      options = URLPatternOptions::FromJsObject(env, args[1].As<Object>());
      if (!options) {
        // If options does not have a value, we assume an error was
        // thrown and scheduled on the isolate. Return early to
        // propagate it.
        return;
      }
    } else {
      THROW_ERR_INVALID_ARG_TYPE(env,
                                 "second argument must be a string or object");
      return;
    }

    // Only remaining argument can be options.
    if (args.Length() > 2) {
      if (!args[2]->IsObject()) {
        THROW_ERR_INVALID_ARG_TYPE(env, "options must be an object");
        return;
      }
      CHECK(!options.has_value());
      options = URLPatternOptions::FromJsObject(env, args[2].As<Object>());
      if (!options) {
        // If options does not have a value, we assume an error was
        // thrown and scheduled on the isolate. Return early to
        // propagate it.
        return;
      }
    }
  }

  // Either url_pattern_init or input as a string must be provided.
  CHECK_IMPLIES(init.has_value(), !input.has_value());

  std::string_view base_url_view{};
  if (base_url) base_url_view = {base_url->data(), base_url->size()};

  ada::url_pattern_input arg0;
  if (init.has_value()) {
    arg0 = std::move(*init);
  } else {
    arg0 = std::move(*input);
  }

  auto url_pattern = parse_url_pattern<URLPatternRegexProvider>(
      std::move(arg0),
      base_url ? &base_url_view : nullptr,
      options.has_value() ? &options.value() : nullptr);

  if (!url_pattern) {
    THROW_ERR_INVALID_URL_PATTERN(env, "Failed to construct URLPattern");
    return;
  }

  new URLPattern(env, args.This(), std::move(*url_pattern));
}

MaybeLocal<Value> URLPattern::URLPatternInit::ToJsObject(
    Environment* env, const ada::url_pattern_init& init) {
  auto isolate = env->isolate();
  auto context = env->context();
  auto result = Object::New(isolate);

  const auto trySet = [&](auto name, const std::optional<std::string>& val) {
    if (!val) return true;
    Local<Value> temp;
    return ToV8Value(context, *val).ToLocal(&temp) &&
           result->Set(context, name, temp).IsJust();
  };

  if (!trySet(env->protocol_string(), init.protocol) ||
      !trySet(env->username_string(), init.username) ||
      !trySet(env->password_string(), init.password) ||
      !trySet(env->hostname_string(), init.hostname) ||
      !trySet(env->port_string(), init.port) ||
      !trySet(env->pathname_string(), init.pathname) ||
      !trySet(env->search_string(), init.search) ||
      !trySet(env->hash_string(), init.hash) ||
      !trySet(env->base_url_string(), init.base_url)) {
    return {};
  }
  return result;
}

std::optional<ada::url_pattern_init> URLPattern::URLPatternInit::FromJsObject(
    Environment* env, Local<Object> obj) {
  ada::url_pattern_init init{};
  Local<String> components[] = {
      env->protocol_string(),
      env->username_string(),
      env->password_string(),
      env->hostname_string(),
      env->port_string(),
      env->pathname_string(),
      env->search_string(),
      env->hash_string(),
      env->base_url_string(),
  };
  auto isolate = env->isolate();
  const auto set_parameter = [&](std::string_view key, std::string_view value) {
    if (key == "protocol") {
      init.protocol = std::string(value);
    } else if (key == "username") {
      init.username = std::string(value);
    } else if (key == "password") {
      init.password = std::string(value);
    } else if (key == "hostname") {
      init.hostname = std::string(value);
    } else if (key == "port") {
      init.port = std::string(value);
    } else if (key == "pathname") {
      init.pathname = std::string(value);
    } else if (key == "search") {
      init.search = std::string(value);
    } else if (key == "hash") {
      init.hash = std::string(value);
    } else if (key == "baseURL") {
      init.base_url = std::string(value);
    }
  };
  Local<Value> value;
  for (const auto& component : components) {
    Utf8Value key(isolate, component);
    if (obj->Get(env->context(), component).ToLocal(&value)) {
      if (value->IsString()) {
        Utf8Value utf8_value(isolate, value);
        set_parameter(key.ToStringView(), utf8_value.ToStringView());
      }
    } else {
      // If ToLocal failed then we assume an error occurred,
      // bail out early to propagate the error.
      return std::nullopt;
    }
  }
  return init;
}

MaybeLocal<Object> URLPattern::URLPatternComponentResult::ToJSObject(
    Environment* env, const ada::url_pattern_component_result& result) {
  auto isolate = env->isolate();
  auto context = env->context();
  auto parsed_group = Object::New(isolate);
  for (const auto& [group_key, group_value] : result.groups) {
    Local<Value> key;
    if (!ToV8Value(context, group_key).ToLocal(&key)) {
      return {};
    }
    Local<Value> value;
    if (group_value) {
      if (!ToV8Value(env->context(), *group_value).ToLocal(&value)) {
        return {};
      }
    } else {
      value = Undefined(isolate);
    }
    if (parsed_group->Set(context, key, value).IsNothing()) {
      return {};
    }
  }
  Local<Value> input;
  if (!ToV8Value(env->context(), result.input).ToLocal(&input)) {
    return {};
  }
  Local<Name> names[] = {env->input_string(), env->groups_string()};
  Local<Value> values[] = {input, parsed_group};
  DCHECK_EQ(arraysize(names), arraysize(values));
  return Object::New(
      isolate, Object::New(isolate), names, values, arraysize(names));
}

MaybeLocal<Value> URLPattern::URLPatternResult::ToJSValue(
    Environment* env, const ada::url_pattern_result& result) {
  auto isolate = env->isolate();
  Local<Name> names[] = {
      env->inputs_string(),
      env->protocol_string(),
      env->username_string(),
      env->password_string(),
      env->hostname_string(),
      env->port_string(),
      env->pathname_string(),
      env->search_string(),
      env->hash_string(),
  };
  LocalVector<Value> inputs(isolate, result.inputs.size());
  size_t index = 0;
  for (auto& input : result.inputs) {
    if (std::holds_alternative<std::string_view>(input)) {
      auto input_str = std::get<std::string_view>(input);
      if (!ToV8Value(env->context(), input_str).ToLocal(&inputs[index])) {
        return {};
      }
    } else {
      DCHECK(std::holds_alternative<ada::url_pattern_init>(input));
      auto init = std::get<ada::url_pattern_init>(input);
      if (!URLPatternInit::ToJsObject(env, init).ToLocal(&inputs[index])) {
        return {};
      }
    }
    index++;
  }
  LocalVector<Value> values(isolate, arraysize(names));
  values[0] = Array::New(isolate, inputs.data(), inputs.size());
  if (!URLPatternComponentResult::ToJSObject(env, result.protocol)
           .ToLocal(&values[1]) ||
      !URLPatternComponentResult::ToJSObject(env, result.username)
           .ToLocal(&values[2]) ||
      !URLPatternComponentResult::ToJSObject(env, result.password)
           .ToLocal(&values[3]) ||
      !URLPatternComponentResult::ToJSObject(env, result.hostname)
           .ToLocal(&values[4]) ||
      !URLPatternComponentResult::ToJSObject(env, result.port)
           .ToLocal(&values[5]) ||
      !URLPatternComponentResult::ToJSObject(env, result.pathname)
           .ToLocal(&values[6]) ||
      !URLPatternComponentResult::ToJSObject(env, result.search)
           .ToLocal(&values[7]) ||
      !URLPatternComponentResult::ToJSObject(env, result.hash)
           .ToLocal(&values[8])) {
    return {};
  }
  return Object::New(
      isolate, Object::New(isolate), names, values.data(), values.size());
}

std::optional<ada::url_pattern_options>
URLPattern::URLPatternOptions::FromJsObject(Environment* env,
                                            Local<Object> obj) {
  ada::url_pattern_options options{};
  Local<Value> ignore_case;
  if (obj->Get(env->context(), env->ignore_case_string())
          .ToLocal(&ignore_case)) {
    if (!ignore_case->IsBoolean()) {
      THROW_ERR_INVALID_ARG_TYPE(env, "options.ignoreCase must be a boolean");
      return std::nullopt;
    }
    options.ignore_case = ignore_case->IsTrue();
  } else {
    // If ToLocal returns false, the assumption is that getting the
    // ignore_case_string threw an error, let's propagate that now
    // by returning std::nullopt.
    return std::nullopt;
  }

  return options;
}

// Perform value lookup and cache the result in a v8::Global.
#define URL_PATTERN_COMPONENT_GETTERS(uppercase_name, lowercase_name)          \
  MaybeLocal<Value> URLPattern::uppercase_name() {                             \
    auto isolate = env()->isolate();                                           \
    if (lowercase_name.IsEmpty()) {                                            \
      Local<Value> value;                                                      \
      if (ToV8Value(env()->context(), url_pattern_.get_##lowercase_name())     \
              .ToLocal(&value)) {                                              \
        lowercase_name.Reset(isolate, value);                                  \
        return value;                                                          \
      }                                                                        \
      return {};                                                               \
    }                                                                          \
    return lowercase_name.Get(isolate);                                        \
  }
URL_PATTERN_COMPONENTS(URL_PATTERN_COMPONENT_GETTERS)
#undef URL_PATTERN_COMPONENT_GETTERS

bool URLPattern::HasRegExpGroups() const {
  return url_pattern_.has_regexp_groups();
}

// Instance methods

MaybeLocal<Value> URLPattern::Exec(Environment* env,
                                   const ada::url_pattern_input& input,
                                   std::optional<std::string_view>& baseURL) {
  if (auto result = url_pattern_.exec(input, baseURL ? &*baseURL : nullptr)) {
    if (result->has_value()) {
      return URLPatternResult::ToJSValue(env, result->value());
    }
    return Null(env->isolate());
  }
  return {};
}

bool URLPattern::Test(Environment* env,
                      const ada::url_pattern_input& input,
                      std::optional<std::string_view>& baseURL) {
  if (auto result = url_pattern_.test(input, baseURL ? &*baseURL : nullptr)) {
    return *result;
  }
  THROW_ERR_OPERATION_FAILED(env, "Failed to test URLPattern");
  return false;
}

// V8 Methods

void URLPattern::Exec(const FunctionCallbackInfo<Value>& args) {
  URLPattern* url_pattern;
  ASSIGN_OR_RETURN_UNWRAP(&url_pattern, args.This());
  auto env = Environment::GetCurrent(args);

  ada::url_pattern_input input;
  std::optional<std::string> baseURL{};
  std::string input_base;
  if (args.Length() == 0) {
    input = ada::url_pattern_init{};
  } else if (args[0]->IsString()) {
    Utf8Value input_value(env->isolate(), args[0].As<String>());
    input_base = input_value.ToString();
    input = std::string_view(input_base);
  } else if (args[0]->IsObject()) {
    auto maybeInput = URLPatternInit::FromJsObject(env, args[0].As<Object>());
    if (!maybeInput.has_value()) return;
    input = std::move(*maybeInput);
  } else {
    THROW_ERR_INVALID_ARG_TYPE(
        env, "URLPattern input needs to be a string or an object");
    return;
  }

  if (args.Length() > 1) {
    if (!args[1]->IsString()) {
      THROW_ERR_INVALID_ARG_TYPE(env, "baseURL must be a string");
      return;
    }
    Utf8Value base_url_value(env->isolate(), args[1].As<String>());
    baseURL = base_url_value.ToStringView();
  }

  Local<Value> result;
  std::optional<std::string_view> baseURL_opt =
      baseURL ? std::optional<std::string_view>(*baseURL) : std::nullopt;
  if (!url_pattern->Exec(env, input, baseURL_opt).ToLocal(&result)) {
    THROW_ERR_OPERATION_FAILED(env, "Failed to exec URLPattern");
    return;
  }
  args.GetReturnValue().Set(result);
}

void URLPattern::Test(const FunctionCallbackInfo<Value>& args) {
  URLPattern* url_pattern;
  ASSIGN_OR_RETURN_UNWRAP(&url_pattern, args.This());
  auto env = Environment::GetCurrent(args);

  ada::url_pattern_input input;
  std::optional<std::string> baseURL{};
  std::string input_base;
  if (args.Length() == 0) {
    input = ada::url_pattern_init{};
  } else if (args[0]->IsString()) {
    Utf8Value input_value(env->isolate(), args[0].As<String>());
    input_base = input_value.ToString();
    input = std::string_view(input_base);
  } else if (args[0]->IsObject()) {
    auto maybeInput = URLPatternInit::FromJsObject(env, args[0].As<Object>());
    if (!maybeInput.has_value()) return;
    input = std::move(*maybeInput);
  } else {
    THROW_ERR_INVALID_ARG_TYPE(
        env, "URLPattern input needs to be a string or an object");
    return;
  }

  if (args.Length() > 1) {
    if (!args[1]->IsString()) {
      THROW_ERR_INVALID_ARG_TYPE(env, "baseURL must be a string");
      return;
    }
    Utf8Value base_url_value(env->isolate(), args[1].As<String>());
    baseURL = base_url_value.ToStringView();
  }

  std::optional<std::string_view> baseURL_opt =
      baseURL ? std::optional<std::string_view>(*baseURL) : std::nullopt;
  args.GetReturnValue().Set(url_pattern->Test(env, input, baseURL_opt));
}

#define URL_PATTERN_COMPONENT_GETTERS(uppercase_name, lowercase_name)          \
  void URLPattern::uppercase_name(const FunctionCallbackInfo<Value>& info) {   \
    URLPattern* url_pattern;                                                   \
    ASSIGN_OR_RETURN_UNWRAP(&url_pattern, info.This());                        \
    Local<Value> result;                                                       \
    if (url_pattern->uppercase_name().ToLocal(&result)) {                      \
      info.GetReturnValue().Set(result);                                       \
    }                                                                          \
  }
URL_PATTERN_COMPONENTS(URL_PATTERN_COMPONENT_GETTERS)
#undef URL_PATTERN_COMPONENT_GETTERS

void URLPattern::HasRegexpGroups(const FunctionCallbackInfo<Value>& info) {
  URLPattern* url_pattern;
  ASSIGN_OR_RETURN_UNWRAP(&url_pattern, info.This());
  info.GetReturnValue().Set(url_pattern->HasRegExpGroups());
}

static void RegisterExternalReferences(ExternalReferenceRegistry* registry) {
  registry->Register(URLPattern::New);
#define URL_PATTERN_COMPONENT_GETTERS(uppercase_name, _)                       \
  registry->Register(URLPattern::uppercase_name);
  URL_PATTERN_COMPONENTS(URL_PATTERN_COMPONENT_GETTERS)
#undef URL_PATTERN_COMPONENT_GETTERS
  registry->Register(URLPattern::HasRegexpGroups);
  registry->Register(URLPattern::Exec);
  registry->Register(URLPattern::Test);
}

static void Initialize(Local<Object> target,
                       Local<Value> unused,
                       Local<Context> context,
                       void* priv) {
  Environment* env = Environment::GetCurrent(context);
  Isolate* isolate = env->isolate();
  auto attributes = static_cast<PropertyAttribute>(ReadOnly | DontDelete);
  auto ctor_tmpl = NewFunctionTemplate(isolate, URLPattern::New);
  auto instance_template = ctor_tmpl->InstanceTemplate();
  auto prototype_template = ctor_tmpl->PrototypeTemplate();
  ctor_tmpl->SetClassName(FIXED_ONE_BYTE_STRING(isolate, "URLPattern"));

  // The signature is used to prevent the property accessors from being
  // called on the wrong receiver object (`this`)
  auto signature = Signature::New(isolate, ctor_tmpl);

  instance_template->SetInternalFieldCount(URLPattern::kInternalFieldCount);

#define ENV_GETTER(lowercase_name) env->lowercase_name##_string()
#define URL_PATTERN_COMPONENT_GETTERS(uppercase_name, lowercase_name)          \
  prototype_template->SetAccessorProperty(                                     \
      ENV_GETTER(lowercase_name),                                              \
      FunctionTemplate::New(                                                   \
          isolate, URLPattern::uppercase_name, Local<Value>(), signature),     \
      Local<FunctionTemplate>(),                                               \
      attributes);
  URL_PATTERN_COMPONENTS(URL_PATTERN_COMPONENT_GETTERS)
#undef URL_PATTERN_COMPONENT_GETTERS
#undef ENV_GETTER

  prototype_template->SetAccessorProperty(
      env->has_regexp_groups_string(),
      FunctionTemplate::New(
          isolate, URLPattern::HasRegexpGroups, Local<Value>(), signature),
      Local<FunctionTemplate>(),
      attributes);

  SetProtoMethodNoSideEffect(isolate, ctor_tmpl, "exec", URLPattern::Exec);
  SetProtoMethodNoSideEffect(isolate, ctor_tmpl, "test", URLPattern::Test);
  SetConstructorFunction(context, target, "URLPattern", ctor_tmpl);
}

}  // namespace node::url_pattern

NODE_BINDING_CONTEXT_AWARE_INTERNAL(url_pattern, node::url_pattern::Initialize)
NODE_BINDING_EXTERNAL_REFERENCE(url_pattern,
                                node::url_pattern::RegisterExternalReferences)
