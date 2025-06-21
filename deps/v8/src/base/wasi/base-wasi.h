// WASI-specific V8 base configuration
#ifndef V8_BASE_WASI_BASE_WASI_H_
#define V8_BASE_WASI_BASE_WASI_H_

#ifdef __wasi__

namespace v8 {
namespace base {

// Forward declare the templates that WASI has issues with
template<typename Dest, typename Source>
inline Dest bit_cast(const Source& source) {
  return *reinterpret_cast<const Dest*>(&source);
}

template<typename T>
using ContextualClass = class T;

}  // namespace base
}  // namespace v8

#endif  // __wasi__

#endif  // V8_BASE_WASI_BASE_WASI_H_
