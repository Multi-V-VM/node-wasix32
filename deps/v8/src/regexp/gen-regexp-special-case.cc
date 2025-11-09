// WASI stub of generated special-case Unicode sets for regexp case folding.
// This mirrors the structure expected by V8 when V8_INTL_SUPPORT is enabled.

#include "src/regexp/special-case.h"

#ifdef V8_INTL_SUPPORT

#include "src/base/lazy-instance.h"

namespace v8 {
namespace internal {

icu::UnicodeSet BuildIgnoreSet() {
  icu::UnicodeSet set;
  // Minimal set derived from upstream generator; matches wasi-gen-regexp-special-case.cc
  set.add(0xdf);
  set.add(0x17f);
  set.add(0x390);
  set.add(0x3b0);
  set.add(0x3f4);
  set.add(0x1e9e);
  set.add(0x1f80, 0x1faf);
  set.add(0x1fb3);
  set.add(0x1fbc);
  set.add(0x1fc3);
  set.add(0x1fcc);
  set.add(0x1fd3);
  set.add(0x1fe3);
  set.add(0x1ff3);
  set.add(0x1ffc);
  set.add(0x2126);
  set.add(0x212a, 0x212b);
  set.add(0xfb05, 0xfb06);
  set.freeze();
  return set;
}

struct IgnoreSetData {
  IgnoreSetData() : set(BuildIgnoreSet()) {}
  const icu::UnicodeSet set;
};

const icu::UnicodeSet& RegExpCaseFolding::IgnoreSet() {
  static base::LazyInstance<IgnoreSetData>::type set = LAZY_INSTANCE_INITIALIZER;
  return set.Pointer()->set;
}

icu::UnicodeSet BuildSpecialAddSet() {
  icu::UnicodeSet set;
  set.add(0x4b);
  set.add(0x53);
  set.add(0x6b);
  set.add(0x73);
  set.add(0xc5);
  set.add(0xe5);
  set.add(0x398);
  set.add(0x3a9);
  set.add(0x3b8);
  set.add(0x3c9);
  set.add(0x3d1);
  set.freeze();
  return set;
}

struct SpecialAddSetData {
  SpecialAddSetData() : set(BuildSpecialAddSet()) {}
  const icu::UnicodeSet set;
};

const icu::UnicodeSet& RegExpCaseFolding::SpecialAddSet() {
  static base::LazyInstance<SpecialAddSetData>::type set = LAZY_INSTANCE_INITIALIZER;
  return set.Pointer()->set;
}

}  // namespace internal
}  // namespace v8

#endif  // V8_INTL_SUPPORT

// Stub main() for WASI builds where this is compiled as an executable
// In normal V8 builds, this would be a code generator tool that outputs
// the IgnoreSet and SpecialAddSet data. For WASI, we've hardcoded the data above.
#ifdef __wasi__
int main(int argc, char** argv) {
  // This stub exists only to satisfy the linker when building gen-regexp-special-case
  // as an executable for WASI. The actual special-case data is hardcoded above.
  // If run, just exit successfully without generating any output.
  return 0;
}
#endif

