#ifndef SRC_WASI_ASYNC_CONTEXT_FIX_H_
#define SRC_WASI_ASYNC_CONTEXT_FIX_H_

#ifdef __wasi__

#include "node.h"

// WASI fix: async_context struct has async_id_value field but code expects async_id
// Provide inline accessor functions to bridge the gap

namespace node {

inline double& async_context_get_async_id(async_context& ctx) {
  return ctx.async_id_value;
}

inline const double& async_context_get_async_id(const async_context& ctx) {
  return ctx.async_id_value;
}

// For code that uses async_context_.async_id or asyncContext.async_id,
// replace with async_context_get_async_id(async_context_) or async_context_get_async_id(asyncContext)

} // namespace node

// Macro to help with the replacement
#define ASYNC_CONTEXT_ID(ctx) node::async_context_get_async_id(ctx)

#endif // __wasi__

#endif // SRC_WASI_ASYNC_CONTEXT_FIX_H_