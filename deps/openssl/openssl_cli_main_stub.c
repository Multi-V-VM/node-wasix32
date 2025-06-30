// Stub main for openssl-cli - WASI build
#include <stdio.h>

int main(int argc, char** argv) {
    printf("openssl-cli stub for WASI build\n");
    // In a real WASI build, we'd skip this tool entirely
    // For now, just exit successfully
    return 0;
}
