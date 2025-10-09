#ifndef V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_
#define V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_

// This file fixes the filesystem::path hash specialization issue in WASI builds
// The error "cannot specialize a dependent template" occurs when trying to
// specialize std::hash<filesystem::path> inside a namespace that makes it dependent

// Ensure we're in global namespace before including filesystem
#ifdef __wasi__
// Close any open v8 namespace before including standard library headers
}  // close any v8 namespace if open

namespace std {}  // Touch std namespace to ensure it exists

// Include filesystem in global namespace context
#include <filesystem>

// Now we can safely use filesystem::path without specialization issues
// The standard library should already provide std::hash<filesystem::path>

// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {

#endif // __wasi__

#endif // V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_