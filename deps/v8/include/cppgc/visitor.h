#ifndef CPPGC_VISITOR_H_
#define CPPGC_VISITOR_H_
#ifdef __wasi__
namespace cppgc {
class Visitor {
 public:
  struct Key {};
  explicit Visitor(Key) {}
  virtual ~Visitor() = default;
  virtual void Visit(const void* ref) {}
};
struct TraceDescriptor {
  const void* base_object_payload;
  void (*trace_callback)(Visitor*, const void*);
};
template <typename T>
struct TraceTrait;
}  // namespace cppgc
#endif
#endif
