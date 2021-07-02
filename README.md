## CachedQueueFetcher

CachedQueueFetcher is a very light weight wrapper, used for dequeing a FIFO
in a smarter way.

### The Problem

Often we deal with FIFO queue, and sometimes we have access only to the
receiving end of queue. While dequeing element from the queue, we commonly
face a problem that once we dequeue an element, we cannot access it again
from the queue unless stored in some variable. Imagine we want to access
3rd last dequeued element again for some other processing. Imagine if
we need to look ahead 3 consecutive dequeued elements for some operation,
then it becomes a bit difficult to manage the history of dequeued elements.

### Solution

CachedQueueFetcher is a wrapper on top of queue.dequeue() method which
internally maintains a small cache (with default size 8). This cache tracks the
history of dequeued elements, so that we can access them again. Also this
utility encapsulate the ability of looking ahead in queue by dequeuing the
elements in advance, storing in local cache and serve them from local cache
when accessed again.

### Example:

```C++


AbstractQueue q;  // Any queue with .Pop() method.
CachedQueueFetcher c_fetcher(q);

x1 = c_fetcher.Pop();
x2 = c_fetcher.Pop();
// Output of x1 and x2 would be identical to what we will get by using q.Pop() twice.

x3 = c_fetcher.Get(-1); assert(x3 == x2); // Access a historically dequeued value.
x4 = c_fetcher.Get(-2); assert(x4 == x1);

x5 = c_fetcher.Get(0); // Look ahead next value in queue without dequeuing.
x6 = c_fetcher.Get(1); // Look ahead next value in queue without dequeuing.
x7 = c_fetcher.Get(2); // Look ahead next value in queue without dequeuing.

// Values of x5, x6, x7 would be identical to what we will get on 3rd, 4th and 5th q.Pop()

x8  = c_fetcher.Pop(); assert(x8  == x5);
x9  = c_fetcher.Pop(); assert(x9  == x6);
x10 = c_fetcher.Pop(); assert(x10 == x7);

assert(c_fetcher.Get(-1) == x10);
assert(c_fetcher.Get(-2) == x9);
assert(c_fetcher.Get(-3) == x8);

```

### Cache overflow

In case of cache overflow, historical values will be flushed one by one.


`c_fetcher.Get(index)` requires `0 <= index < cache_capacity` for looking
ahead (fetching future values) and `-index <= history_size` for looking
historical values.

Every `c_fetcher.Pop()` adds a value in history to increase `history_size` by 1
(bounded by cache_capacity). In case of overflow, old history enteries will be
deleted.
Assuming cache_capacity = 8 (default), after doing `c_fetcher.Pop()` 50 times,
we can use `c_fetcher.Get(-1)`, `c_fetcher.Get(-2)`, ..., `c_fetcher.Get(-8)` 
for accessing last 8 dequed values, but we cannot use `c_fetcher.Get(-9)`.
The `c_fetcher.Get(-9)` will throw exception "Not enough history".

Note that we are using same cache for storing future values. Which means using
`c_fetcher.Get(4)` might flush older history, because future values cannot be
flushed unless they are poped using `c_fetcher.Pop()`. That's why
`c_fetcher.Get(index)` requires `0 <= index < cache_capacity` for looking ahead.


### Performance Overhead Due to This Wrapper

CachedQueueFetcher have negligible overhead.

Space complexity : This wrapper stores a very small cache of default size 8

Time complexity: The time complexity overhead of all the method of
CachedQueueFetcher is O(1) (Which is also independent of cache_capacity).

