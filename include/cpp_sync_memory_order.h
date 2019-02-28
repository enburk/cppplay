TEST_OFF // https://en.cppreference.com/w/cpp/atomic/atomic_fetch_add
{
    auto sleep = []() { std::this_thread::sleep_for (std::chrono::milliseconds (randint (0,5))); };
    // meaning of cnt: (for N == 5)
    //  5: there are no active readers or writers.
    //  1...4: there are 4...1 readers active, The writer is blocked
    //  0: temporary value between fetch_sub and fetch_add in reader lock
    // -1: there is a writer active. The readers are blocked.
    const int N = 3; // four concurrent readers are allowed
    const int M = 5; // number of writes
    std::atomic<int> cnt(N);
    std::vector<int> data;
    std::memory_order order = std::memory_order_relaxed; // std::memory_order_seq_cst;
 
    auto reader  = [&](int id)
    {
        for(;;)
        {
            // lock
            while (cnt.fetch_sub(1, order) <= 0)
                   cnt.fetch_add(1, order);
            // read
            if (!data.empty())
                cout << ( "reader " + std::to_string(id)
                         + " sees " + std::to_string(*data.rbegin()) + '\n');
            if (data.size() == M) break;
            // unlock
            cnt.fetch_add(1, order);
            sleep ();
        }
    };
 
    auto writer = [&]()
    {
        for(int n = 0; n < M; ++n)
        {
            // lock
            while (cnt.fetch_sub(N+1, order) != N)
                   cnt.fetch_add(N+1, order);
            // write
            data.push_back(n); cout << "writer pushed back " << n << '\n';
            // unlock
            cnt.fetch_add(N+1, order);
            sleep ();
        }
    };
 
    std::vector<std::thread> v;
    for (int n = 0; n < N; ++n)
    v.emplace_back(reader, n);
    v.emplace_back(writer);
    for (auto& t : v) t.join();
};
