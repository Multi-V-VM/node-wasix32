#ifdef __wasi__

#include "uv.h"
#include "internal.h"

// WASI stub for interface enumeration - return empty list
int uv_interface_addresses(uv_interface_address_t** addresses, int* count) {
  *addresses = NULL;
  *count = 0;
  return 0;
}

void uv_free_interface_addresses(uv_interface_address_t* addresses, int count) {
  (void)addresses;
  (void)count;
  // No-op for WASI
}

#endif /* __wasi__ */
