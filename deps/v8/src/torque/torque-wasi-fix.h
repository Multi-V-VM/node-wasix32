#ifndef V8_TORQUE_WASI_FIX_H_
#define V8_TORQUE_WASI_FIX_H_

namespace v8 {
namespace internal {
namespace torque {

// Forward declaration if SourcePosition is not available
#ifndef SOURCE_POSITION_DEFINED
struct SourcePosition {
  int line;
  int column;
  SourcePosition() : line(0), column(0) {}
  SourcePosition(int l, int c) : line(l), column(c) {}
};
#define SOURCE_POSITION_DEFINED
#endif

// Alias for compatibility
using SourceLocation = SourcePosition;

}  // namespace torque
}  // namespace internal
}  // namespace v8

#endif  // V8_TORQUE_WASI_FIX_H_
