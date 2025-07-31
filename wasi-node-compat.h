#ifndef WASI_NODE_COMPAT_H_
#define WASI_NODE_COMPAT_H_

// Master compatibility header for Node.js WASI builds
// Include this at the beginning of Node.js source files that have WASI compilation issues

#ifdef __wasi__

// Standard library includes needed by compatibility headers
#include <vector>
#include <string>
#include <cstring>

// System compatibility must come first
#include "wasi-system-compat.h"

// V8 sandbox stubs must come before V8 headers
#include "wasi-v8-sandbox-stubs.h"

// simdutf compatibility
#include "wasi-simdutf-compat.h"

// V8 API stubs - include this before V8 headers if needed
// Note: This may already be included via wasi-v8-api-additions.h

// NOTE: V8 headers should be included here by the source file

// V8 missing methods - include this after V8 headers
// This should be included by source files AFTER including V8 headers
// #include "wasi-v8-missing-methods.h"

// Additional WASI compatibility definitions

// Missing list node structures for Node.js
namespace node {

template<typename T>
struct ListNode {
  ListNode* prev_;
  ListNode* next_;
  T* container;
  
  ListNode() : prev_(nullptr), next_(nullptr), container(nullptr) {}
};

template<typename T, ListNode<T> T::*member>
struct ListHead {
  ListNode<T>* head_;
  
  ListHead() : head_(nullptr) {}
};

} // namespace node

#endif // __wasi__

#endif // WASI_NODE_COMPAT_H_