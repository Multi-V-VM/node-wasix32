// Copyright 2024 the V8 project authors. All rights reserved.
// WASI build base type includes
#ifndef V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_
#define V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_

#ifdef __wasi__
// No-op: avoid pulling base headers through this umbrella; include what you
// need directly in each TU to prevent nested-namespace resolution issues.
#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_BASE_INCLUDES_H_
