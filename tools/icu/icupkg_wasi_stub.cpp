// WASI stub for icupkg tool
// This is a minimal stub that just copies input to output for WASI builds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Parse basic arguments to find input and output files
    const char* input_file = nullptr;
    const char* output_file = nullptr;
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!input_file) {
                input_file = argv[i];
            } else if (!output_file) {
                output_file = argv[i];
            }
        }
    }
    
    if (!input_file || !output_file) {
        fprintf(stderr, "icupkg stub: need input and output files\n");
        return 1;
    }
    
    // Simple file copy for WASI stub
    FILE* in = fopen(input_file, "rb");
    if (!in) {
        fprintf(stderr, "icupkg stub: cannot open input file %s\n", input_file);
        return 1;
    }
    
    FILE* out = fopen(output_file, "wb");
    if (!out) {
        fprintf(stderr, "icupkg stub: cannot create output file %s\n", output_file);
        fclose(in);
        return 1;
    }
    
    // Copy file
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, bytes, out) != bytes) {
            fprintf(stderr, "icupkg stub: write error\n");
            fclose(in);
            fclose(out);
            return 1;
        }
    }
    
    fclose(in);
    fclose(out);
    
    printf("icupkg stub: copied %s to %s\n", input_file, output_file);
    return 0;
}