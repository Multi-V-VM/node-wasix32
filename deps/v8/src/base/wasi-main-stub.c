// Dummy main for WASI tools that need a main symbol
#ifdef __wasi__
int main(int argc, char** argv) {
    // This is a stub main for tools that shouldn't be executed directly
    return 0;
}
#endif
