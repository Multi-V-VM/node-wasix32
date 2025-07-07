// Host stub for icupkg when cross-compiling for WASI
// This runs on the host system (macOS/Linux) to process ICU data files
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char* argv[]) {
  // For WASI cross-compilation, we just copy the data file as-is
  // The real icupkg would handle endianness conversion, but for WASI
  // we'll handle that at build time if needed
  
  if (argc < 3) {
    fprintf(stderr, "Usage: icupkg [options] input output\n");
    return 1;
  }
  
  // Find input and output files (last two arguments)
  const char* src = argv[argc-2];
  const char* dst = argv[argc-1];
  
  // Check for -t option (endianness)
  bool needs_swap = false;
  for (int i = 1; i < argc-2; i++) {
    if (strncmp(argv[i], "-t", 2) == 0) {
      // -tl = little endian, -tb = big endian
      // For now, we assume the data is already in the correct format
      needs_swap = false;
    }
  }
  
  // Simple file copy using C-style I/O
  FILE* in = fopen(src, "rb");
  if (!in) {
    fprintf(stderr, "Error: Could not open input file: %s\n", src);
    return 1;
  }
  
  FILE* out = fopen(dst, "wb");
  if (!out) {
    fprintf(stderr, "Error: Could not open output file: %s\n", dst);
    fclose(in);
    return 1;
  }
  
  // Copy the file
  char buffer[4096];
  size_t bytes;
  while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0) {
    if (fwrite(buffer, 1, bytes, out) != bytes) {
      fprintf(stderr, "Error: Failed to write output file: %s\n", dst);
      fclose(in);
      fclose(out);
      return 1;
    }
  }
  
  fclose(in);
  fclose(out);
  
  printf("icupkg: Created %s from %s\n", dst, src);
  return 0;
}