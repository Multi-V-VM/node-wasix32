#include <stdio.h>
#include <stdlib.h>

// Export the main function for WebAssembly
extern "C" {
    // WASI entry point
    int _start() {
        printf("Minimal Node.js WebAssembly Implementation\n");
        printf("Hello from WebAssembly!\n");
        return 0;
    }
    
    // Standard main function
    int main(int argc, char* argv[]) {
        printf("Minimal Node.js WebAssembly Implementation\n");
        printf("Arguments: %d\n", argc);
        
        for (int i = 0; i < argc; i++) {
            printf("  arg[%d]: %s\n", i, argv[i]);
        }
        
        printf("Hello from WebAssembly!\n");
        return 0;
    }
}
