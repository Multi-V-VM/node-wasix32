#ifndef WASI_V8_CUSTOM_ARGUMENTS_H_
#define WASI_V8_CUSTOM_ARGUMENTS_H_

// Do not define stubs unless explicitly requested. Upstream V8 defines
// CustomArguments in src/api/api-arguments.h; accidental inclusion of a stub
// causes redefinition errors.
#ifdef WASI_USE_CUSTOM_ARGUMENTS_STUB
namespace v8 {
namespace internal {

// CustomArguments template for WASI builds
template <typename T>
class CustomArguments {
 public:
  CustomArguments() = default;
  ~CustomArguments() = default;
  
  // Basic stub implementation
  void* operator[](int /*index*/) { return nullptr; }
  int length() const { return 0; }
};

// Friend function stubs
inline void PrintFunctionCallbackInfo(void* /*info*/) {}
inline void PrintPropertyCallbackInfo(void* /*info*/) {}

} // namespace internal
} // namespace v8
#endif  // WASI_USE_CUSTOM_ARGUMENTS_STUB

#endif  // WASI_V8_CUSTOM_ARGUMENTS_H_
