#ifndef WASI_PLATFORM_FIXES_H
#define WASI_PLATFORM_FIXES_H

// Additional platform compatibility fixes for WASI

namespace v8 {

// CreateParams additional members
inline void FixCreateParams(Isolate::CreateParams& params) {
  // Add dummy members for WASI compatibility
  struct {
    size_t max_young_generation_size_in_bytes = 0;
  } constraints;
  
  params.constraints = constraints;
  params.array_buffer_allocator_shared = nullptr;
}

} // namespace v8

#endif // WASI_PLATFORM_FIXES_H