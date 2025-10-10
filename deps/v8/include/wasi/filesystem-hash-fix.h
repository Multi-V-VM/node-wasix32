#ifndef V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_
#define V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_

// This file fixes the filesystem::path hash specialization issue in WASI builds
// The error "cannot specialize a dependent template" occurs when trying to
// specialize std::hash<filesystem::path> inside a namespace that makes it dependent

#ifdef __wasi__

#include <filesystem>

#endif  // __wasi__

#endif // V8_INCLUDE_WASI_FILESYSTEM_HASH_FIX_H_
