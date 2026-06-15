#ifndef WASI_V8_MISSING_TYPES_H_
#define WASI_V8_MISSING_TYPES_H_

#ifdef __wasi__

// This header includes canonical v8::base definitions needed by WASI builds.
// It must not create v8::internal::base, because that shadows ::v8::base for
// V8 code compiled inside namespace v8::internal.

#include <cstdint>
#include <cstdio>
#include <functional>
#include <iterator>
#include <utility>
#include <memory>
#include <cstdlib>

// Only include V8 internal headers when compiling V8 source code itself
// (detected by V8_BASE_EXPORT being defined from v8config.h internal usage)
// External code including v8.h won't have these internal headers available
#ifdef V8_EXPORT_PRIVATE
// Include the actual V8 base headers (these paths work when V8 include paths are set up)
#include "src/base/hashmap.h"
#include "src/base/pointer-with-payload.h"
#include "src/base/threaded-list.h"
#include "src/base/bit-field.h"
#include "src/base/bits.h"
#include "src/base/atomicops.h"
#include "src/base/atomic-utils.h"
#include "src/base/platform/mutex.h"
#include "src/base/vector.h"
#include "src/base/address-region.h"
#include "src/base/virtual-address-space.h"
#include "src/base/discriminated-union.h"
#include "src/base/strings.h"
#include "src/base/hashing.h"
#include "src/base/container-utils.h"
#include "src/base/template-utils.h"
#include "src/base/platform/memory.h"
#include "src/base/platform/wrappers.h"
#endif  // V8_EXPORT_PRIVATE

// V8 internal code uses unqualified base:: names from inside v8::internal,
// which must resolve outward to the canonical ::v8::base namespace. Do not
// create a v8::internal::base shim here; it shadows ::v8::base and breaks
// ordinary C++ namespace lookup in compiler/Turboshaft headers.

// Note: Local<> implementation has been moved to v8-data.h
// Note: Global Address alias has been moved to v8-data.h

#endif  // __wasi__

#endif  // WASI_V8_MISSING_TYPES_H_
