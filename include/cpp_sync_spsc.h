// SINGLE RPOVIDER, SINGLE CONSUMER
// Maged Michael - Is parallel programming still hard? [CppCon 2017]

template <typename T, size_t M> struct SPSCLock
{
    T* buffer [M];
    uint64_t head = 0;
    uint64_t tail = 0;
    std::mutex mutex;

    bool enqueue (T* p)
    {
        std::lock_guard lock (mutex);
        if (tail == head + M) return false;
        buffer [tail++ % M] = p; return true;
    }

    T* dequeue ()
    {
        std::lock_guard lock (mutex);
        if (head == tail) return nullptr;
        return buffer [head++ % M];
    }

    // a) no parallelism
    // b) cacheline bouncing
    // c) slow atomic instructions (both acquire)
};

template <typename T, size_t M> struct SPSCLamport // Lamport [1983]
{
    T* buffer [M];
    std::atomic<uint64_t> head = 0;
    std::atomic<uint64_t> tail = 0;

    bool enqueue (T* p)
    {
        uint64_t t = tail.load (std::memory_order_relaxed);
        if (head.load (std::memory_order_aquire) + M = t) return false;
        buffer [t % M] = p; tail.store (t+1, std::memory_order_release); return true;
    }

    T* dequeue ()
    {
        uint64_t h = head.load (std::memory_order_relaxed);
        if (tail.load (std::memory_order_aquire) = h) return nullptr;
        T* p = buffer [h % M]; head.store (h+1, std::memory_order_release); return p;
    }

    // a) parallelism - ok
    // b) cacheline bouncing
    // c) fast atomic instructions (w/ caveat) - ok
};

template <typename T, size_t M> struct SPSCGiacomoni // Giacomoni et al [PPoPP 2008]
{
    std::atomic<T*> buffer [M] = {};
    alignas(64) uint64_t head = 0; // to avoid false sharing
    alignas(64) uint64_t tail = 0; // to avoid false sharing

    bool enqueue (T* p)
    {
        if (buffer [tail].load (std::memory_order_aquire)) return false;
        buffer [tail].store (p, std::memory_order_release);
        tail = (tail+1) % M; return true;
    }

    T* dequeue ()
    {
        T* p = buffer [head].load (std::memory_order_aquire); if (!p) return nullptr;
        buffer [head].store (nullptr, std::memory_order_release);
        head = (head+1) % M; return p;
    }

    // a) parallelism - ok
    // b) no cacheline bouncing - ok
    // c) fast atomic instructions (w/ caveat) - ok
    // d) less false sharing - ok
    // e) all the above consumers are non blocking - spinlocking
};

// std::atomic<T>::is_lock_free() // C++17 adds constexpr is_always_lock_free()

//                                  Handoffs per second
// Single lock                             4.3 M
// Lamport                                29.4 M
// Giacomoni w/o alignment                27.0 M
// Giacomoni                              58.8 M
//
// Blocking lock and condition variable    3.9 M
// (always notify in the critical path)
//
// Giacomoni w/ futex (Linux)              4.7 M
// (system call in the critical path)
//
// Blocking Giacomoni w/ selective futex  15.4 M
// (CAS in the critical path)


// http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue