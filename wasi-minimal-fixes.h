#ifndef WASI_MINIMAL_FIXES_H
#define WASI_MINIMAL_FIXES_H

// Minimal fixes for WASI compilation - only define what's truly missing

namespace v8 {
namespace internal {
namespace base {
    // Only create aliases if they don't already exist
    // The V8 build system should handle most of these
}}}

// Only define if not already defined
#ifndef kSystemPointerSize
constexpr int kSystemPointerSize = sizeof(void*);
#endif

#endif // WASI_MINIMAL_FIXES_H