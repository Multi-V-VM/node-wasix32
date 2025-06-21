#ifndef CPPGC_HEAP_STATISTICS_H_
#define CPPGC_HEAP_STATISTICS_H_
#ifdef __wasi__
namespace cppgc {
  struct HeapStatistics {
    enum class DetailLevel { kBrief, kDetailed };
  };
}
#endif
#endif
