#!/bin/bash

# Node.js WebAssembly (WASI/Wasix) Cross-Compilation Build Script
# 
# This script properly configures the environment and builds Node.js as WebAssembly for WASI/Wasix
# Cross-compilation from host platform to wasm32 architecture
#
# Usage: ./build-node-wasix.sh
# Output: WebAssembly binary at out/Release/node

set -e

echo "🚀 Building Node.js for WebAssembly (WASI/Wasix) - Cross-compilation" 

# Check if we're in the right directory
if [ ! -f "configure.py" ]; then
    echo "❌ Error: Must run this from the Node.js source directory"
    exit 1
fi

# Check for required tools
echo "🔧 Checking prerequisites..."
if ! command -v /Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/wasm32-wasi-clang" 2>/dev/null; then
    echo "❌ WASI SDK clang not found. Please install it:"
    echo "   brew install mise && mise install wasm-sdk"
    echo "   export PATH=\"/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin:$PATH\""
    exit 1
fi

if ! command -v /Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/wasm32-wasi-clang++" 2>/dev/null; then
    echo "❌ WASI SDK clang++ not found. Please install it:"
    echo "   brew install mise && mise install wasm-sdk"
    echo "   export PATH=\"/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin:$PATH\""
    exit 1
fi

if ! command -v /Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/llvm-ar" 2>/dev/null; then
    echo "❌ llvm-ar not found. Please install it:"
    echo "   brew install mise && mise install wasm-sdk" 
    echo "   export AR=\"/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/llvm-ar:$PATH\""
    exit 1
fi

echo "✅ All required tools found!"

# Export LLVM version environment variables  
export LLVM_AR="/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/llvm-ar"
export LLVM_NM="/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/llvm-nm"  
export LLVM_RANLIB="/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/llvm-ranlib"

# Set up the WASI cross-compilation environment with proper sysroot ordering
# wasix-sysroot first for header priority and WASI SDK sysroot second
export CC="/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/wasm32-wasi-clang --sysroot=./wasix-sysroot --sysroot=/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/share/wasi-sysroot/ -D_WASI_EMULATED_SIGNAL -DCPPFLAGS='-I./wasix-sysroot/include -I/Users/toni/Labs/wasi-labs/node-wasix32' -Wl,--allow-undefined"

export CXX="/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/bin/wasm32-wasi-clang++ --sysroot=./wasix-sysroot --sysroot=/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/share/wasi-sysroot/ -D_WASI_EMULATED_SIGNAL -DCPPFLAGS='-I./wasix-sysroot/include -I/Users/toni/Labs/wasi-labs/node-wasix32' -nostdinc++ -isystem /Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/share/wasi-sysroot/include' -DCPPFLAGS='-I/Users/toni/Labs/wasi-labs/node-wasix32'"

# Include paths - wasix-sysroot first for proper C/C++ header resolution
export CFLAGS="-I./wasix-sysroot/include -I../ -I../src"
export CXXFLAGS="-I./wasix-sysroot/include -nostdinc++ -isystem /Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/share/wasi-sysroot/include -I./wasix-sysroot/include -DCPPFLAGS='-I/Users/toni/Labs/wasi-labs/node-wasix32'"

# Linker configuration for WebAssembly
export LDFLAGS="-L/Users/toni/.local/share/mise/installs/wasi-sdk/28/wasi-sdk/share/wasi-sysroot/lib -lwasi-emulated-signal -Wl,--allow-undefined"

# Signal emulation flags to be added to CFLAGS and LDFLAGS  
export CPPFLAGS="-I./wasix-sysroot/include -I../src -D_WASI_EMULATED_SIGNAL"

# Architecture configuration - WASI cross-compilation requires wasm32 to be supported
export DEST_CPU="wasm32"  
export HOST_ARCH="wasm32"

# Disable components not supporting WebAssembly yet
export WITHOUT_INSPECTOR=1
export WITHOUT_INTL=1
export WITHOUT_SSL=1

echo "⚙️  Configuring Node.js for wasm32 cross-compilation..."
./configure \
    --dest-cpu=$DEST_CPU \
    --dest-host=$HOST_ARCH \
    --cross-compiling \
    $WITHOUT_INSPECTOR \
    $WITHOUT_INTL \
    $WITHOUT_SSL

echo "🔨 Building Node.js as WebAssembly with $(nproc) cores..."
make -j$(nproc)

# Check if build succeeded
if [ -f "out/Release/node" ]; then
    echo "🎉 Build successful!"
    echo "📦 WebAssembly binary created: out/Release/node"
    echo "🔍 Test with: file out/Release/node"
    file out/Release/node"
    echo "⚡ Run with: wasmeruntime out/Release/node"
else
    echo "❌ Build failed!"
    cd out && ls -la out/Release/
fi

echo "🚨 Done!"
