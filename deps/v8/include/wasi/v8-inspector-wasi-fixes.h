#ifndef V8_INSPECTOR_WASI_FIXES_H_
#define V8_INSPECTOR_WASI_FIXES_H_

// Fix V8_EXPORT macro for WASI builds
#ifndef V8_EXPORT
#define V8_EXPORT
#endif

// Fix MessageErrorLevel for WASI builds
namespace v8 {
// Forward declaration to avoid conflicts
class Isolate;

// Define MessageErrorLevel in the v8 namespace
enum MessageErrorLevel {
  kMessageLog = (1 << 0),
  kMessageDebug = (1 << 1),
  kMessageInfo = (1 << 2),
  kMessageError = (1 << 3),
  kMessageWarning = (1 << 4),
  kMessageAll = kMessageLog | kMessageDebug | kMessageInfo | kMessageError | kMessageWarning
};
} // namespace v8

#endif // V8_INSPECTOR_WASI_FIXES_H_