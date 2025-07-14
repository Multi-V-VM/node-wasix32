// Copyright 2017 The Abseil Authors.
// WASM stub implementation of low_level_alloc.cc

#include "absl/base/internal/low_level_alloc.h"

#ifdef __wasm__

#include <cstdlib>
#include <cstring>
#include <new>

#include "absl/base/config.h"
#include "absl/base/internal/raw_logging.h"

namespace absl {
ABSL_NAMESPACE_BEGIN
namespace base_internal {

// Simple WASM implementation that delegates to malloc/free
struct LowLevelAlloc::Arena {
  uint32_t flags;
  int allocation_count;
  
  explicit Arena(uint32_t flags_value) : flags(flags_value), allocation_count(0) {}
};

// Static storage for default arenas
static LowLevelAlloc::Arena default_arena(0);
static LowLevelAlloc::Arena unhooked_arena(0);

LowLevelAlloc::Arena *LowLevelAlloc::DefaultArena() {
  return &default_arena;
}

LowLevelAlloc::Arena *LowLevelAlloc::NewArena(uint32_t flags) {
  // For WASM, we'll use malloc to allocate the arena
  Arena *arena = new Arena(flags);
  return arena;
}

bool LowLevelAlloc::DeleteArena(Arena *arena) {
  if (arena == &default_arena || arena == &unhooked_arena) {
    return false;
  }
  if (arena->allocation_count != 0) {
    return false;
  }
  delete arena;
  return true;
}

void *LowLevelAlloc::Alloc(size_t request) {
  return AllocWithArena(request, DefaultArena());
}

void *LowLevelAlloc::AllocWithArena(size_t request, Arena *arena) {
  if (request == 0) {
    return nullptr;
  }
  
  // Add space for metadata
  size_t total_size = request + sizeof(Arena*) + sizeof(size_t);
  void *result = std::malloc(total_size);
  
  if (result == nullptr) {
    ABSL_RAW_LOG(FATAL, "LowLevelAlloc::Alloc failed");
    return nullptr;
  }
  
  // Store arena pointer and size at the beginning
  Arena **arena_ptr = static_cast<Arena**>(result);
  *arena_ptr = arena;
  
  size_t *size_ptr = reinterpret_cast<size_t*>(arena_ptr + 1);
  *size_ptr = request;
  
  arena->allocation_count++;
  
  // Return pointer after metadata
  return static_cast<char*>(result) + sizeof(Arena*) + sizeof(size_t);
}

void LowLevelAlloc::Free(void *v) {
  if (v == nullptr) {
    return;
  }
  
  // Get the actual allocation start
  void *actual_ptr = static_cast<char*>(v) - sizeof(Arena*) - sizeof(size_t);
  
  // Get the arena
  Arena **arena_ptr = static_cast<Arena**>(actual_ptr);
  Arena *arena = *arena_ptr;
  
  arena->allocation_count--;
  
  std::free(actual_ptr);
}

}  // namespace base_internal
ABSL_NAMESPACE_END
}  // namespace absl

#endif  // __wasm__