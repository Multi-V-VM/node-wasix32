#ifndef WASI_V8_BITS_FIXES_H_
#define WASI_V8_BITS_FIXES_H_

#ifdef __wasi__

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>

// Include V8 base headers
#include "src/base/bit-field.h"
#include "src/base/bits.h"

// Avoid defining make_array here; it is provided by src/base/template-utils.h

#endif  // __wasi__

#endif  // WASI_V8_BITS_FIXES_H_
