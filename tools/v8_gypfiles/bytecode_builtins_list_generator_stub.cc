// WASI stub for bytecode_builtins_list_generator that copies a pre-generated
// header containing the bytecode handler definitions. This avoids executing the
// full C++ generator (which depends on host-only build flags) during the WASI
// cross-compile.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace {
std::string ResolveTemplatePath(const char* exe_path) {
  if (exe_path == nullptr) {
    return "wasi-bytecodes-builtins-list.h";
  }

  std::string path(exe_path);
  const char kNeedle[] = "/out/";
  std::size_t pos = path.rfind(kNeedle);
  if (pos == std::string::npos) {
    return "wasi-bytecodes-builtins-list.h";
  }
  std::string root = path.substr(0, pos);
  return root + "/wasi-bytecodes-builtins-list.h";
}

bool CopyFile(const char* source_path, const char* dest_path) {
  FILE* in = std::fopen(source_path, "rb");
  if (!in) return false;
  FILE* out = std::fopen(dest_path, "wb");
  if (!out) {
    std::fclose(in);
    return false;
  }

  char buffer[4096];
  std::size_t read = 0;
  while ((read = std::fread(buffer, 1, sizeof(buffer), in)) > 0) {
    if (std::fwrite(buffer, 1, read, out) != read) {
      std::fclose(in);
      std::fclose(out);
      return false;
    }
  }
  std::fclose(in);
  std::fclose(out);
  return true;
}
}  // namespace

int main(int argc, char* argv[]) {
  if (argc != 2) return 1;

  const std::string template_path = ResolveTemplatePath(argv[0]);
  if (!CopyFile(template_path.c_str(), argv[1])) {
    return 1;
  }
  return 0;
}
