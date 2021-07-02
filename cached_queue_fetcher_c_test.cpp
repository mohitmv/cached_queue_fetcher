#include <queue>
#include <iostream>

#include "cached_queue_fetcher.h"

int q_pop(std::queue<int>* q) {
  auto output = q->front();
  q->pop();
  return output;
}

CachedQueueFetcherTemplate(CachedQueueFetcher, std::queue<int>, q_pop, int, 4);

void Test1() {
  std::queue<int> q;
  for (auto x : {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140}) {
    q.push(x);
  }
  CachedQueueFetcher c_fetcher;
  InitCachedQueueFetcher(&c_fetcher, &q);
  assert(PopCachedQueueFetcher(&c_fetcher) == 10);
  assert(PopCachedQueueFetcher(&c_fetcher) == 20);
  assert(PopCachedQueueFetcher(&c_fetcher) == 30);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 40);
  assert(GetCachedQueueFetcher(&c_fetcher, -1) == 30);
  assert(GetCachedQueueFetcher(&c_fetcher, -2) == 20);
  assert(GetCachedQueueFetcher(&c_fetcher, -3) == 10);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 40);
  assert(GetCachedQueueFetcher(&c_fetcher, -1) == 30);
  assert(GetCachedQueueFetcher(&c_fetcher, -2) == 20);
  assert(GetCachedQueueFetcher(&c_fetcher, -3) == 10);
  assert(PopCachedQueueFetcher(&c_fetcher) == 40);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 50);
  assert(GetCachedQueueFetcher(&c_fetcher, 1) == 60);
  assert(GetCachedQueueFetcher(&c_fetcher, 2) == 70);
  assert(PopCachedQueueFetcher(&c_fetcher) == 50);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 60);
  assert(GetCachedQueueFetcher(&c_fetcher, 1) == 70);
  assert(GetCachedQueueFetcher(&c_fetcher, 2) == 80);
  assert(PopCachedQueueFetcher(&c_fetcher) == 60);
  assert(PopCachedQueueFetcher(&c_fetcher) == 70);
  assert(GetCachedQueueFetcher(&c_fetcher, 1) == 90);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 80);
  assert(GetCachedQueueFetcher(&c_fetcher, -1) == 70);
  assert(GetCachedQueueFetcher(&c_fetcher, -2) == 60);
  assert(GetCachedQueueFetcher(&c_fetcher, 0) == 80);
  assert(GetCachedQueueFetcher(&c_fetcher, 1) == 90);
  assert(GetCachedQueueFetcher(&c_fetcher, 2) == 100);
  assert(GetCachedQueueFetcher(&c_fetcher, 3) == 110);
  printf("Test1 Successful\n");
}

void Test2() {
  std::queue<int> q;
  for (auto x : {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140}) {
    q.push(x);
  }
  CachedQueueFetcher c_fetcher;
  InitCachedQueueFetcher(&c_fetcher, &q);
  assert(PopCachedQueueFetcher(&c_fetcher) == 10);
  assert(PopCachedQueueFetcher(&c_fetcher) == 20);
  assert(PopCachedQueueFetcher(&c_fetcher) == 30);
  assert(PopCachedQueueFetcher(&c_fetcher) == 40);
  assert(PopCachedQueueFetcher(&c_fetcher) == 50);
  assert(PopCachedQueueFetcher(&c_fetcher) == 60);
  assert(PopCachedQueueFetcher(&c_fetcher) == 70);
  assert(PopCachedQueueFetcher(&c_fetcher) == 80);
  assert(PopCachedQueueFetcher(&c_fetcher) == 90);
  printf("Test2 Successful\n");
}


int main() {
  Test1();
  Test2();
}
