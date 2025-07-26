#ifndef V8_WASI_FIXES_H_
#define V8_WASI_FIXES_H_

#ifdef __wasi__

// Fix for v8-inspector.h expecting Isolate::MessageErrorLevel
namespace v8 {
class Isolate;

// Define MessageErrorLevel enum that v8-inspector.h expects
enum IsolateMessageErrorLevel {
  kMessageLog = (1 << 0),
  kMessageDebug = (1 << 1),
  kMessageInfo = (1 << 2),
  kMessageError = (1 << 3),
  kMessageWarning = (1 << 4),
  kMessageAll = kMessageLog | kMessageDebug | kMessageInfo | kMessageError | kMessageWarning
};
} // namespace v8

// Macro to fix namespace issues
#define V8_ISOLATE_MESSAGE_ERROR_LEVEL v8::IsolateMessageErrorLevel

// Fix Isolate static methods
namespace v8 {
inline Isolate* GetCurrentIsolate() { return nullptr; }
} // namespace v8

#endif // __wasi__

#endif // V8_WASI_FIXES_H_