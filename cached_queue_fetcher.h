// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef CACHED_QUEUE_FETCHER_H_
#define CACHED_QUEUE_FETCHER_H_

#define CachedQueueFetcherTemplate(name, fetcher_expr, element_type, cache_capacity) \
struct name {
  element_type cache[cache_capacity];
  int len;
  int ptr;
  int num_advanced_fetch;
};

void Init##name(name* cf) {
  cf->len = 0;
  cf->ptr = 0;
  cf->num_advanced_fetch = 0;
}

element_type Pop##name(name*cf) {
  if (cf->num_advanced_fetch == 0) {
    InternalFetch##name(1);
  }
  auto output = InternalGetCacheValue##name(cf->len - cf->num_advanced_fetch);
  cf->num_advanced_fetch--;
  return output;
}

element_type Get##name(name*cf, int index) {
  if (index < 0) {
    if (len - cf->num_advanced_fetch >= -index) {
      return InternalGetCacheValue##names(
          cf, cf->len - cf->num_advanced_fetch + index);
    } else {
      printf("Not enough history for accessing Get(%d)\n", index);
      exit(1);
    }
  } else if (index < cache_capacity) {
    if (index >= cf->num_advanced_fetch) {
      InternalFetch##name(cf, index - cf->num_advanced_fetch + 1);
    }
    return InternalGetCacheValue##name(
        cf, cf->len - cf->num_advanced_fetch + index);
  } else {
    printf("Can't Get values more advanced than cache_capacity. Tried Get(%d)\n", index);
    exit(1);
  }
}


void InternalFetch##name(name* cf, int n) {
    assert(n >= 0);
    assert(n + cf->num_advanced_fetch <= cache_capacity);
    cf->num_advanced_fetch += n;
    for (int i = 0 ; i < n; i++) {
      InternalPushInCache##name(cf, fetcher_expr);
    }
}

// Push in cache queue. if the queue is full, flush the first element before
// pushing new element.
void InternalPushInCache##name(name* cf, element_type e) {
  cf->cache[(cf->ptr + cf->len) % cache_capacity] = e;
  if (cf->len < cache_capacity) {
    cf->len++;
  } else {
    cf->ptr = (cf->ptr + 1) % cache_capacity;
  }
}

// Return i'th value in the 
element_type InternalGetCacheValue##name(const name* cf, int i) {
  return cf->cache[(cf->ptr + i) % cache_capacity];
}

#endif  // CACHED_QUEUE_FETCHER_H_
