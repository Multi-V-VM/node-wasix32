#ifndef SRC_WASI_BASE_OBJECT_LIST_FIX_H_
#define SRC_WASI_BASE_OBJECT_LIST_FIX_H_

#ifdef __wasi__

// Fix for BaseObjectList iterator in WASI builds
template<typename T>
inline auto begin(const T& list) -> decltype(list.begin()) {
  return list.begin();
}

template<typename T>
inline auto end(const T& list) -> decltype(list.end()) {
  return list.end();
}

#endif // __wasi__

#endif // SRC_WASI_BASE_OBJECT_LIST_FIX_H_
