#ifndef TORQUE_STUBS_H_
#define TORQUE_STUBS_H_

#ifdef __wasi__
// Minimal stubs for torque components

namespace v8 {
namespace internal {
namespace torque {

// Forward declarations
class SourcePosition;
class SourceId; 
class LineAndColumn;
class Declarable;

// Minimal LanguageServerData stub
class LanguageServerData {
public:
  static void AddDefinition(SourcePosition token, SourcePosition definition) {}
  static SourcePosition FindDefinition(SourceId source, LineAndColumn pos) { return SourcePosition(); }
  static void PrepareAllDeclarableSymbols() {}
};

} // namespace torque
} // namespace internal  
} // namespace v8

// Stub macro for WASI
#define EXPORT_CONTEXTUAL_VARIABLE(...)

#endif // __wasi__

#endif // TORQUE_STUBS_H_
