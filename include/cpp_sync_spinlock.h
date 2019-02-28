TEST_OFF // https://en.cppreference.com/w/cpp/atomic/atomic_flag
{
    int nn = 0;

    std::atomic_flag lock = ATOMIC_FLAG_INIT;
 
    auto f = [&](int n)
    {
        for (int cnt = 1; cnt <= 10; ++cnt) {
            while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
                 ; // spin
            std::cout << ++nn << " thread " << n << " count " << cnt << '\n';
            lock.clear(std::memory_order_release);               // release lock
        }
    };
 
    std::vector<std::thread> v;
    for (int n = 0; n < 3; ++n) v.emplace_back(f, n);
    for (auto& t : v) t.join();
};

// Possible output:
// 
// 1 thread 0 count 1
// 2 thread 1 count 1
// 3 thread 2 count 1
// 4 thread 1 count 2
// 5 thread 0 count 2
// 6 thread 2 count 2
// 7 thread 0 count 3
// 8 thread 2 count 3
// 9 thread 2 count 4
// 10 thread 0 count 4
// 11 thread 2 count 5
// 12 thread 1 count 3
// 13 thread 1 count 4
// 14 thread 0 count 5
// 15 thread 0 count 6
// 16 thread 2 count 6
// 17 thread 1 count 5
// 18 thread 1 count 6
// 19 thread 1 count 7
// 20 thread 1 count 8
// 21 thread 1 count 9
// 22 thread 2 count 7
// 23 thread 0 count 7
// 24 thread 1 count 10
// 25 thread 2 count 8
// 26 thread 2 count 9
// 27 thread 0 count 8
// 28 thread 0 count 9
// 29 thread 0 count 10
// 30 thread 2 count 10
