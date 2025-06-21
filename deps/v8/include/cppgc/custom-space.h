#ifndef CPPGC_CUSTOM_SPACE_H_
#define CPPGC_CUSTOM_SPACE_H_
#ifdef __wasi__
namespace cppgc {
  class CustomSpaceBase {};
  enum class CustomSpaceIndex : size_t {};
}
#endif
#endif
