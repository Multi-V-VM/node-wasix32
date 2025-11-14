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

// Fix for branch protection not supported in WASI
#pragma push_macro("mbranch-protection")
#define mbranch-protection

#undef mbranch-protection
#define mbranch-protection

// Fix for unsupported libc++ features
#pragma push_macro("no-exceptions")
#define no-exceptions

#undef no-exceptions
#define no-exceptions

#pragma pop_macro("no-exceptions")

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
