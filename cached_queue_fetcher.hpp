// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef CACHED_QUEUE_FETCHER_HPP_
#define CACHED_QUEUE_FETCHER_HPP_

namespace quick {

template<typename Fetcher, int cache_capacity=8>
class CachedQueueFetcher {
 public:
  using ElementType = decltype(std::declval<Fetcher>()());
  CachedQueueFetcher(Fetcher fetcher) : fetcher_(std::move(fetcher)) { }
  ElementType Pop() {
    if (num_advanced_fetch == 0) {
      Fetch(1);
    }
    auto output = GetCacheValue(len - num_advanced_fetch);
    num_advanced_fetch--;
    return output;
  }
  ElementType Get(int index) {
    if (index < 0) {
      if (len - num_advanced_fetch >= -index) {
        return GetCacheValue(len - num_advanced_fetch + index);
      } else {
        throw std::runtime_error("Not enough history");
      }
    } else if (index < cache_capacity) {
      if (index >= num_advanced_fetch) {
        Fetch(index - num_advanced_fetch + 1);
      }
      return GetCacheValue(len - num_advanced_fetch + index);
    } else {
      throw std::runtime_error("Can't Get values more advanced than cache_capacity");
    }
  }

  void PrintDebugString() const {
    std::cout << "Q[" << len << "]: ";
    for (int i = 0; i < len; i++) {
      std::cout << GetCacheValue(i) << ", ";
    }
    std::cout << "\n,num_advanced_fetch = " << num_advanced_fetch << "\n";
    std::cout << std::endl;
  }

 private:
  void Fetch(int n) {
    assert(n >= 0);
    assert(n + num_advanced_fetch <= cache_capacity);
    num_advanced_fetch += n;
    for (int i = 0 ; i < n; i++) {
      PushInCache(fetcher_());
    }
  }
  // Push in cache queue. if the queue is full, flush the first element before
  // pushing new element.
  void PushInCache(ElementType e) {
    cache[(ptr + len) % cache_capacity] = e;
    if (len < cache_capacity) {
      len++;
    } else {
      ptr = (ptr + 1) % cache_capacity;
    }
  }
  // Return i'th value in the 
  ElementType GetCacheValue(int i) const {
    return cache[(ptr + i) % cache_capacity];
  }
  Fetcher fetcher_;
  ElementType cache[cache_capacity];
  int len = 0;
  int ptr = 0;
  int num_advanced_fetch = 0;
};

}  // namespace quick

#endif  // CACHED_QUEUE_FETCHER_HPP_
