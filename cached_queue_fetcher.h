// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef CACHED_QUEUE_FETCHER_H_
#define CACHED_QUEUE_FETCHER_H_

#include <assert.h>

#define CachedQueueFetcherTemplate(name, q_type, q_pop, element_type, cache_capacity) \
struct name {                                                                  \
  element_type cache[cache_capacity];                                          \
  int len;                                                                     \
  int ptr;                                                                     \
  int num_advanced_fetch;                                                      \
  q_type* q;                                                                   \
};                                                                             \
                                                                               \
void Init##name(name* cf, q_type* q) {                                         \
  cf->len = 0;                                                                 \
  cf->ptr = 0;                                                                 \
  cf->num_advanced_fetch = 0;                                                  \
  cf->q = q;                                                                   \
}                                                                              \
                                                                               \
/* Push in cache queue. if the queue is full, flush the first element */       \
/* before pushing new element. */                                              \
void InternalPushInCache##name(name* cf, element_type e) {                     \
  cf->cache[(cf->ptr + cf->len) % cache_capacity] = e;                         \
  if (cf->len < cache_capacity) {                                              \
    cf->len++;                                                                 \
  } else {                                                                     \
    cf->ptr = (cf->ptr + 1) % cache_capacity;                                  \
  }                                                                            \
}                                                                              \
                                                                               \
/* Return i'th value in the */                                                 \
element_type InternalGetCacheValue##name(const name* cf, int i) {              \
  return cf->cache[(cf->ptr + i) % cache_capacity];                            \
}                                                                              \
                                                                               \
void InternalFetch##name(name* cf, int n) {                                    \
    assert(n >= 0);                                                            \
    assert(n + cf->num_advanced_fetch <= cache_capacity);                      \
    cf->num_advanced_fetch += n;                                               \
    for (int i = 0 ; i < n; i++) {                                             \
      InternalPushInCache##name(cf, q_pop(cf->q));                             \
    }                                                                          \
}                                                                              \
                                                                               \
element_type Pop##name(name*cf) {                                              \
  if (cf->num_advanced_fetch == 0) {                                           \
    InternalFetch##name(cf, 1);                                                \
  }                                                                            \
  auto output = InternalGetCacheValue##name(                                   \
      cf, cf->len - cf->num_advanced_fetch);                                   \
  cf->num_advanced_fetch--;                                                    \
  return output;                                                               \
}                                                                              \
                                                                               \
element_type Get##name(name*cf, int index) {                                   \
  if (index < 0) {                                                             \
    if (cf->len - cf->num_advanced_fetch >= -index) {                          \
      return InternalGetCacheValue##name(                                      \
          cf, cf->len - cf->num_advanced_fetch + index);                       \
    } else {                                                                   \
      printf("Not enough history for accessing Get(%d)\n", index);             \
      exit(1);                                                                 \
    }                                                                          \
  } else if (index < cache_capacity) {                                         \
    if (index >= cf->num_advanced_fetch) {                                     \
      InternalFetch##name(cf, index - cf->num_advanced_fetch + 1);             \
    }                                                                          \
    return InternalGetCacheValue##name(                                        \
        cf, cf->len - cf->num_advanced_fetch + index);                         \
  } else {                                                                     \
    printf("Can't Get values more advanced than cache_capacity. ");            \
    printf("Tried Get(%d)\n", index);                                          \
    exit(1);                                                                   \
  }                                                                            \
}

#endif  // CACHED_QUEUE_FETCHER_H_
