// WASI stub for bytecode_builtins_list_generator
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
  // For WASI builds, create a minimal bytecodes-builtins-list.h
  if (argc > 1) {
    FILE* out = fopen(argv[1], "w");
    if (out) {
      fprintf(out, "// WASI stub - bytecodes-builtins-list.h\n");
      fprintf(out, "#ifndef V8_BUILTINS_BUILTINS_LIST_H_\n");
      fprintf(out, "#define V8_BUILTINS_BUILTINS_LIST_H_\n");
      fprintf(out, "// Minimal stub for WASI builds\n");
      fprintf(out, "#endif  // V8_BUILTINS_BUILTINS_LIST_H_\n");
      fclose(out);
    }
  }
  return 0;
}