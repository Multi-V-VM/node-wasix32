// Simple WebAssembly module with exported functions

// Define functions with proper export attributes
__attribute__((export_name("add")))
int add(int a, int b) {
    return a + b;
}

__attribute__((export_name("get_answer")))
int get_answer() {
    return 42;
}

__attribute__((export_name("double_number")))
int double_number(int x) {
    return x * 2;
}
