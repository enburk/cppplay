using std::experimental::generator;

// CppCon 2018: G. Nishanov “Nano-coroutines to the Rescue! (Using Coroutines TS, of Course)”

generator<char> hello ()
{
    for (char c : "Hello, World!\n")
    {
        std::this_thread::sleep_for (100ms); co_yield c;
    }
}

//std::experimental::
Concurrency::
task<void>
sleepy ()
{
    cout << "Going to sleep..." << endl; //co_await std::this_thread::sleep_for (1s);
    cout << "Woke up."          << endl; //co_return 42;
}

// __builtin_prefetch((const void*)(prefetch_address),0,0); // GCC

// void _mm_prefetch (char const* p, int hint); // VS
// #define _MM_HINT_NTA    0 // prefetch data to a closest cache to CPU minimizing cache pollution
// #define _MM_HINT_T0     1 // prefetch data into all levels of the cache
// #define _MM_HINT_T1     2 // prefetch data into level 2 cache and higher
// #define _MM_HINT_T2     3 // prefetch data into level 3 cache and higher
// #define _MM_HINT_ENTA   4 // 

template<
    typename Iterator,
    typename Value,
    typename Found,
    typename NotFound
>
auto binary_search (Iterator first, Iterator last, Value value, Found on_found, NotFound on_not_found) -> Concurrency::task<void>
{
    auto len = last - first; while (len > 0)
    {
        auto half = len / 2;
        auto middle = first + half;
        auto middle_key = co_await prefetch (*middle);
        if (middle_key < value) {
            first = middle + 1;
            len = len - half -1;
        } else
            len = half;
        if (middle_key == value) 
            co_return on_found (value, middle);
    }
    co_return on_not_found (value);
}

TEST_OFF
{
//    for (char c : hello ()) cout << c;

    //cout << sleepy ().get () << endl;
};
