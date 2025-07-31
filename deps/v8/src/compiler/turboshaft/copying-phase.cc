#ifdef __wasi__
// WASI compatibility - must be included before any other headers
#include <stdint.h>
#include <stddef.h>
#include <vector>

namespace v8 {
namespace internal {

// Missing sandbox constants
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;
constexpr size_t kCodePointerTableReservationSize = 1024 * 1024;
constexpr uint32_t kMaxExternalPointers = 65536;
constexpr uint32_t kMaxCodePointers = 65536;
constexpr uint32_t kMaxCapacity = 65536;
constexpr size_t kCodePointerTableEntrySize = 8;
constexpr int kExternalPointerTagShift = 48;

using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;
constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
constexpr CodePointerHandle kNullCodePointerHandle = 0;

} // namespace internal
} // namespace v8

namespace v8 {
// LocalVector template
template<typename T> class Local;
template<typename T>
class LocalVector : public std::vector<Local<T>> {
 public:
  explicit LocalVector(Isolate* isolate) {}
};
} // namespace v8
#endif // __wasi__

// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/compiler/turboshaft/copying-phase.h"

namespace v8::internal::compiler::turboshaft {

int CountDecimalDigits(uint32_t value) {
  int result = 1;
  while (value > 9) {
    result++;
    value = value / 10;
  }
  return result;
}

std::ostream& operator<<(std::ostream& os, PaddingSpace padding) {
  if (padding.spaces > 10000) return os;
  for (int i = 0; i < padding.spaces; ++i) {
    os << ' ';
  }
  return os;
}

}  // namespace v8::internal::compiler::turboshaft
