#ifndef WASI_V8_VALUE_METHODS_H
#define WASI_V8_VALUE_METHODS_H

// This file provides additional V8 Value methods for WASI builds
// Note: Most Value methods are now implemented directly in v8-value.h with __wasi__ guards
// This file is kept for compatibility but methods have been moved to v8-value.h

namespace v8 {

// All Value method implementations have been moved to v8-value.h to avoid redefinition conflicts
// This file is now primarily used for compatibility

} // namespace v8

#endif // WASI_V8_VALUE_METHODS_H