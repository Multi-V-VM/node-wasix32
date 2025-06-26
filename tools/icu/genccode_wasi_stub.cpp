// WASI stub for genccode tool
// This generates a minimal C file with ICU data for WASI builds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    const char* input_file = nullptr;
    const char* output_dir = ".";
    const char* entry_point = "icudata";
    const char* name_prefix = "icudt";
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            output_dir = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            entry_point = argv[++i];
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            name_prefix = argv[++i];
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        }
    }
    
    if (!input_file) {
        fprintf(stderr, "genccode stub: need input file\n");
        return 1;
    }
    
    // Read input file
    FILE* in = fopen(input_file, "rb");
    if (!in) {
        fprintf(stderr, "genccode stub: cannot open input file %s\n", input_file);
        return 1;
    }
    
    // Get file size
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    
    // Read file content
    unsigned char* data = (unsigned char*)malloc(file_size);
    if (!data) {
        fprintf(stderr, "genccode stub: out of memory\n");
        fclose(in);
        return 1;
    }
    
    size_t bytes_read = fread(data, 1, file_size, in);
    fclose(in);
    
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "genccode stub: read error\n");
        free(data);
        return 1;
    }
    
    // Generate output filename
    char output_path[1024];
    snprintf(output_path, sizeof(output_path), "%s/%s_dat.c", output_dir, name_prefix);
    
    // Create output C file
    FILE* out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "genccode stub: cannot create output file %s\n", output_path);
        free(data);
        return 1;
    }
    
    // Write C file header
    fprintf(out, "/* genccode stub generated file */\n");
    fprintf(out, "#include <stddef.h>\n");
    fprintf(out, "#ifdef __cplusplus\n");
    fprintf(out, "extern \"C\" {\n");
    fprintf(out, "#endif\n\n");
    
    // Write data array
    fprintf(out, "static const unsigned char %s_data[] = {\n", entry_point);
    
    for (long i = 0; i < file_size; i++) {
        if (i % 16 == 0) {
            fprintf(out, "    ");
        }
        fprintf(out, "0x%02x", data[i]);
        if (i < file_size - 1) {
            fprintf(out, ",");
        }
        if (i % 16 == 15 || i == file_size - 1) {
            fprintf(out, "\n");
        } else {
            fprintf(out, " ");
        }
    }
    
    fprintf(out, "};\n\n");
    
    // Write size variable
    fprintf(out, "const size_t %s_size = %ld;\n", entry_point, file_size);
    
    // Write entry point
    fprintf(out, "const unsigned char* %s = %s_data;\n", entry_point, entry_point);
    
    fprintf(out, "\n#ifdef __cplusplus\n");
    fprintf(out, "}\n");
    fprintf(out, "#endif\n");
    
    fclose(out);
    free(data);
    
    printf("genccode stub: generated %s (%ld bytes)\n", output_path, file_size);
    return 0;
}