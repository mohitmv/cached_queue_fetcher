#include <queue>
#include <iostream>

#include "cached_queue_fetcher.hpp"


#define ASSERT_EXCEPTION(c, e_msg)            \
  {                                           \
    bool failed = false;                      \
    try {                                     \
      c;                                      \
    } catch (const std::exception& e) {       \
      failed = true;                          \
      assert(std::string(e.what()) == e_msg); \
    }                                         \
    assert(failed);                           \
  }

void Test1() {
  std::queue<int> q;
  for (auto x : {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140}) {
    q.push(x);
  }
  auto fetcher = [&](){
    auto output = q.front();
    q.pop();
    return output; };
  using CFetcher = quick::CachedQueueFetcher<decltype(fetcher), 4>;
  CFetcher c_fetcher(fetcher);
  assert(c_fetcher.Pop() == 10);
  assert(c_fetcher.Pop() == 20);
  assert(c_fetcher.Pop() == 30);
  assert(c_fetcher.Get(0) == 40);
  assert(c_fetcher.Get(-1) == 30);
  assert(c_fetcher.Get(-2) == 20);
  assert(c_fetcher.Get(-3) == 10);
  assert(c_fetcher.Get(0) == 40);
  assert(c_fetcher.Get(-1) == 30);
  assert(c_fetcher.Get(-2) == 20);
  assert(c_fetcher.Get(-3) == 10);
  assert(c_fetcher.Pop() == 40);
  assert(c_fetcher.Get(0) == 50);
  assert(c_fetcher.Get(1) == 60);
  assert(c_fetcher.Get(2) == 70);
  assert(c_fetcher.Pop() == 50);
  assert(c_fetcher.Get(0) == 60);
  assert(c_fetcher.Get(1) == 70);
  assert(c_fetcher.Get(2) == 80);
  assert(c_fetcher.Pop() == 60);
  assert(c_fetcher.Pop() == 70);
  assert(c_fetcher.Get(1) == 90);
  assert(c_fetcher.Get(0) == 80);
  assert(c_fetcher.Get(-1) == 70);
  assert(c_fetcher.Get(-2) == 60);
  ASSERT_EXCEPTION(c_fetcher.Get(-3), "Not enough history");
  assert(c_fetcher.Get(0) == 80);
  assert(c_fetcher.Get(1) == 90);
  assert(c_fetcher.Get(2) == 100);
  assert(c_fetcher.Get(3) == 110);
  ASSERT_EXCEPTION(c_fetcher.Get(-1), "Not enough history");
  ASSERT_EXCEPTION(c_fetcher.Get(4),
                   "Can't Get values more advanced than cache_capacity");
  std::cout << "Test1 Successful" << std::endl;
}

void Test2() {
  std::queue<int> q;
  for (auto x : {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140}) {
    q.push(x);
  }
  auto fetcher = [&](){
    auto output = q.front();
    q.pop();
    return output; };
  using CFetcher = quick::CachedQueueFetcher<decltype(fetcher), 4>;
  CFetcher c_fetcher(fetcher);
  assert(c_fetcher.Pop() == 10);
  assert(c_fetcher.Pop() == 20);
  assert(c_fetcher.Pop() == 30);
  assert(c_fetcher.Pop() == 40);
  assert(c_fetcher.Pop() == 50);
  assert(c_fetcher.Pop() == 60);
  assert(c_fetcher.Pop() == 70);
  assert(c_fetcher.Pop() == 80);
  assert(c_fetcher.Pop() == 90);
  std::cout << "Test2 Successful" << std::endl;
}


int main() {
  Test1();
  Test2();
}
