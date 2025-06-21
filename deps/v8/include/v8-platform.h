#ifndef INCLUDE_V8_PLATFORM_H_
#define INCLUDE_V8_PLATFORM_H_

#ifdef __wasi__
// WASI-specific platform implementation

namespace v8 {
class Platform {
public:
  virtual ~Platform() = default;
};
} // namespace v8

#else
// Include real v8-platform.h for non-WASI
#endif

#endif // INCLUDE_V8_PLATFORM_H_
