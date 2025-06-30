#ifdef V8_TARGET_ARCH_WASM32
#include "../../include/libplatform/libplatform-wasi-fix.h"
#endif
// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef __wasi__
// WASI stub implementation
#include <vector>
#include <string>

namespace v8 {

class Isolate;

namespace platform {
namespace tracing {

class TraceConfig {
 public:
  TraceConfig() {}
  static TraceConfig* CreateDefaultTraceConfig() { return new TraceConfig(); }
  bool IsCategoryGroupEnabled(const char*) const { return false; }
  void AddIncludedCategory(const char*) {}
 private:
  std::vector<std::string> included_categories_;
};

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#else

#include <string.h>

#include "include/libplatform/v8-tracing.h"
#include "src/base/logging.h"

namespace v8 {

class Isolate;

namespace platform {
namespace tracing {

TraceConfig* TraceConfig::CreateDefaultTraceConfig() {
  TraceConfig* trace_config = new TraceConfig();
  trace_config->included_categories_.push_back("v8");
  return trace_config;
}

bool TraceConfig::IsCategoryGroupEnabled(const char* category_group) const {
  std::stringstream category_stream(category_group);
  while (category_stream.good()) {
    std::string category;
    getline(category_stream, category, ',');
    for (const auto& included_category : included_categories_) {
      if (category == included_category) return true;
    }
  }
  return false;
}

void TraceConfig::AddIncludedCategory(const char* included_category) {
  DCHECK(included_category != nullptr && strlen(included_category) > 0);
  included_categories_.push_back(included_category);
}

}  // namespace tracing
}  // namespace platform
}  // namespace v8

#endif // __wasi__
