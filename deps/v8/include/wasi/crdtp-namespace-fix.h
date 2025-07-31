#ifndef V8_INCLUDE_WASI_CRDTP_NAMESPACE_FIX_H_
#define V8_INCLUDE_WASI_CRDTP_NAMESPACE_FIX_H_

// Ensure crdtp namespace is in global scope, not under v8
// This prevents v8::crdtp resolution issues with inspector protocol

// Only define if not already included via inspector protocol headers
#ifndef CRDTP_MAYBE_H_
// Ensure we're not already in v8 namespace
namespace crdtp {}  // Ensure global crdtp exists

// Forward declare crdtp in global namespace to prevent it from being
// resolved as v8::crdtp when included after v8::std namespace mappings
namespace crdtp {
  // Forward declarations to establish namespace in global scope
  class DeserializerState;
  template<typename T, typename> struct ProtocolTypeTraits;
  namespace detail {
    template<typename T> struct MaybeTypedef;
  }
  template<typename T> class span;
  class Serializable;
  // Maybe is a type alias defined in crdtp/maybe.h, not a class template
  // Do not forward declare type aliases
  template<typename T> class ValueMaybe;
}
#endif // CRDTP_MAYBE_H_

// Also ensure ::node namespace is available globally
namespace node {
  namespace inspector {
    namespace protocol {
      // Forward declarations
      class Binary;
    }
  }
}

// Prevent crdtp from being declared inside v8 namespace
namespace v8 {
  // DO NOT declare crdtp namespace here
  // Use ::crdtp for the global namespace
}

// Helper to ensure crdtp types are resolved correctly
#define CRDTP_NAMESPACE ::crdtp

#endif // V8_INCLUDE_WASI_CRDTP_NAMESPACE_FIX_H_