#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("Stub genccode called with %d arguments\n", argc);
    // Create an empty output file if an output file is specified
    if (argc > 1) {
        FILE* f = fopen(argv[argc-1], "w");
        if (f) {
            fprintf(f, "// Stub genccode output\n");
            fclose(f);
        }
    }
    return 0;
}
