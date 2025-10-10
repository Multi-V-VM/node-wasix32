#ifndef WASI_CPPGC_STUBS_H_
#define WASI_CPPGC_STUBS_H_

#ifdef __wasi__
// Intentionally left minimal. cppgc custom space types and SpaceTrait
// are provided by deps/v8/include/cppgc/custom-space.h. Defining them here
// risks conflicting with the canonical definitions depending on include order.
namespace cppgc { class AllocationHandle; }
#endif  // __wasi__

#endif // WASI_CPPGC_STUBS_H_
