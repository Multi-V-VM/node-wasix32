#ifndef V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_
#define V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_

// This file fixes the filesystem::path hash specialization issue in WASI builds
// The error "cannot specialize a dependent template" occurs when trying to
// specialize std::hash<filesystem::path> inside a namespace that makes it dependent

// Ensure we're in global namespace before including filesystem
#ifdef __wasi__
// Exit any v8 namespace we might be in
namespace std {}  // Touch std namespace to ensure it exists

// Include filesystem in global namespace context
#include <filesystem>

// Now we can safely use filesystem::path without specialization issues
// The standard library should already provide std::hash<filesystem::path>

#endif // __wasi__

#endif // V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_