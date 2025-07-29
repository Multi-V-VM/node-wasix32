#ifndef SRC_INSPECTOR_INSPECTOR_NAMESPACE_FIX_H_
#define SRC_INSPECTOR_INSPECTOR_NAMESPACE_FIX_H_

// This file ensures inspector protocol types are resolved correctly
// and prevents namespace resolution issues

// Ensure we're using global crdtp namespace, not v8::crdtp
using ::crdtp::ProtocolTypeTraits;
using ::crdtp::DeserializerState;
using ::crdtp::Serializable;
using ::crdtp::span;
using ::crdtp::detail::MaybeTypedef;

// Ensure protocol types are available
namespace node {
namespace inspector {
namespace protocol {
  class Binary;
  class Value;
  class Object;
}
}
}

#endif // SRC_INSPECTOR_INSPECTOR_NAMESPACE_FIX_H_