#ifndef WASI_V8_ISOLATE_METHODS_H_
#define WASI_V8_ISOLATE_METHODS_H_

#ifdef __wasi__

// This file adds missing Isolate methods for WASI compatibility

namespace v8 {

// Forward declarations
class Message;
class Context;
typedef void (*MessageCallback)(Local<Message> message, Local<Value> data);
typedef void (*FatalErrorCallback)(const char* location, const char* message);
typedef void (*OOMErrorCallback)(const char* location, const OOMDetails& details);
typedef bool (*AbortOnUncaughtExceptionCallback)(Isolate*);

// Extend Isolate with missing methods
class Isolate {
public:
  // Existing methods...
  
  // DisallowJavascriptExecutionScope for WASI
  class DisallowJavascriptExecutionScope {
  public:
    enum OnFailure {
      CRASH_ON_FAILURE,
      THROW_ON_FAILURE
    };
    
    DisallowJavascriptExecutionScope(Isolate* isolate, OnFailure on_failure) {
      // WASI stub - no-op
    }
    
    ~DisallowJavascriptExecutionScope() {
      // WASI stub - no-op
    }
    
    static constexpr OnFailure CRASH_ON_FAILURE = OnFailure::CRASH_ON_FAILURE;
  };
  
  // Add missing methods for WASI
  void AddMessageListenerWithErrorLevel(MessageCallback callback,
                                      int message_levels,
                                      Local<Value> data = Local<Value>()) {
    // WASI stub - no-op
  }
  
  void SetAbortOnUncaughtExceptionCallback(AbortOnUncaughtExceptionCallback callback) {
    // WASI stub - no-op
  }
  
  void SetFatalErrorHandler(FatalErrorCallback callback) {
    // WASI stub - no-op
  }
  
  void SetOOMErrorHandler(OOMErrorCallback callback) {
    // WASI stub - no-op
  }
  
  // CreateParams structure extensions
  struct CreateParams {
    // Add missing fields
    struct {
      size_t max_old_generation_size_in_bytes() const { return 0; }
      void ConfigureDefaults(size_t physical_memory, size_t virtual_memory_limit) {
        // WASI stub - no-op
      }
    } constraints;
    
    int embedder_wrapper_object_index = -1;
    int embedder_wrapper_type_index = -1;
  };
};

} // namespace v8

#endif // __wasi__

#endif // WASI_V8_ISOLATE_METHODS_H_