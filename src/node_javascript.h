#ifndef SRC_NODE_JAVASCRIPT_H_
#define SRC_NODE_JAVASCRIPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// JavaScript module source code declarations
// These will be defined in the generated node_javascript.cc file

extern const char lib_http_agent_js_source[];
extern const size_t lib_http_agent_js_source_len;

extern const char lib_http_client_js_source[];
extern const size_t lib_http_client_js_source_len;

extern const char lib_http_common_js_source[];
extern const size_t lib_http_common_js_source_len;

extern const char lib_buffer_js_source[];
extern const size_t lib_buffer_js_source_len;

extern const char lib_console_js_source[];
extern const size_t lib_console_js_source_len;

extern const char lib_crypto_js_source[];
extern const size_t lib_crypto_js_source_len;

extern const char lib_fs_js_source[];
extern const size_t lib_fs_js_source_len;

extern const char lib_http_js_source[];
extern const size_t lib_http_js_source_len;

extern const char lib_net_js_source[];
extern const size_t lib_net_js_source_len;

extern const char lib_os_js_source[];
extern const size_t lib_os_js_source_len;

extern const char lib_path_js_source[];
extern const size_t lib_path_js_source_len;

extern const char lib_process_js_source[];
extern const size_t lib_process_js_source_len;

extern const char lib_stream_js_source[];
extern const size_t lib_stream_js_source_len;

extern const char lib_util_js_source[];
extern const size_t lib_util_js_source_len;

#ifdef __cplusplus
}
#endif

#endif  // SRC_NODE_JAVASCRIPT_H_