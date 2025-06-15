#include <stdio.h>
#include <stdlib.h>

// Simple WASI-compatible application
int main(int argc, char* argv[]) {
    printf("WASI Node.js WebAssembly Implementation\n");
    printf("Arguments: %d\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("  arg[%d]: %s\n", i, argv[i]);
    }
    
    printf("Hello from WebAssembly!\n");
    return 0;
}
