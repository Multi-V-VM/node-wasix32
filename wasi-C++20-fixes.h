#ifndef WASI_CPP20_FIXES_H_
#define WASI_CPP20_FIXES_H_

#ifdef __wasi__

// Fix for C++20 constexpr issues in wasm32-wasi target
#pragma push_macro("constexpr")
#define constexpr

// Revert constexpr to const for WASI compatibility  
#undef constexpr
#define constexpr const

#pragma pop_macro("constexpr")

// Provide missing C++ standard library headers
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Define nullptr_t for C++ compatibility
#ifdef __cplusplus
namespace std {
    typedef decltype(nullptr) nullptr_t;
}
#endif

// Fix for branch protection not supported in WASI
#pragma push_macro("mbranch_protection")
#define mbranch_protection

#undef mbranch_protection
#define mbranch_protection

// Fix for unsupported libc++ features  
#pragma push_macro("no_exceptions")
#define no_exceptions

#undef no_exceptions
#define no_exceptions

#pragma pop_macro("no_exceptions")

// Fix for missing V8 types in WASI build
namespace v8 {
namespace internal {
    struct SourceLocation {
        const char* function_name_;
        const char* file_name_;
        int line_number_;
        
        SourceLocation() 
            : function_name_("unknown"), file_name_("unknown"), line_number_(0) {}
            
        SourceLocation(const char* function, const char* file, int line)
            : function_name_(function), file_name_(file), line_number_(line) {}
    };
}
}

// Use internal SourceLocation for V8 platform compatibility
using SourceLocation = v8::internal::SourceLocation;

#endif // __wasi__

#endif // WASI_CPP20_FIXES_H_
