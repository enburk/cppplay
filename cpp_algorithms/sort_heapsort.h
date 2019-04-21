template <
typename I,
typename C = std::less<>
>
void heap_sort (I first, I last, C compare = C {})
{
    std::make_heap (first, last, compare);
    std::sort_heap (first, last, compare);
}

template <
typename I,
typename C = std::less<>
> // O(N log N) instead of O(N) of std::make_heap
void naive_make_heap (I first, I last, C compare = C {})
{
    for (auto it = first; it != last;) std::push_heap (first, ++it, compare); 
}

template <
typename I,
typename C = std::less<>
>
void naive_sort_heap (I first, I last, C compare = C {})
{
    for (auto it = last; it != first;) std::pop_heap (first, it--, compare);
}

template <
typename I,
typename C = std::less<>
>
void naive_heap_sort (I first, I last, C compare = C {})
{
    naive_make_heap (first, last, compare);
    naive_sort_heap (first, last, compare);
}

TEST_OFF
{
    {
        auto v1 = random (1'000'000); auto v2 = v1;

        Time t0; std:: make_heap (v1.begin(), v1.end());
        Time t1; naive_make_heap (v2.begin(), v2.end());
        Time t2; std:: sort_heap (v1.begin(), v1.end());
        Time t3; naive_sort_heap (v2.begin(), v2.end());
        Time t4;

        cout << "std:: make_heap " <<  t1-t0 << " sec" << endl;
        cout << "naive_make_heap " <<  t2-t1 << " sec" << endl << endl;
        cout << "std:: sort_heap " <<  t3-t2 << " sec" << endl;
        cout << "naive_sort_heap " <<  t4-t3 << " sec" << endl << endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
    }

    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back ("random       ", random, 1'000'000);
    tests.emplace_back ("few unique   ", random, 1'000'000, 0, 5);
    tests.emplace_back ("sorted       ", sorted, 1'000'000);
    tests.emplace_back ("reversed     ", sorted, 1'000'000, -1.0);
    tests.emplace_back ("nearly sorted", sorted, 1'000'000,  0.8);

    for (auto test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data (); auto v1 = v0, v2 = v0, v3 = v0; 

        Time t0; merge_sort      (v1.begin(), v1.end());
        Time t1; heap_sort       (v2.begin(), v2.end());
        Time t2; naive_heap_sort (v3.begin(), v3.end());
        Time t3;

        cout << "merge sort      " << t1-t0 << " sec" << endl;
        cout << "heap sort       " << t2-t1 << " sec" << endl;
        cout << "heap sort naive " << t3-t2 << " sec" << endl;
        cout <<  endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
        assert (std::is_sorted (v3.begin(), v3.end()));
    }
};

// Possible output:
// 
// std:: make_heap 0.010'756'374 sec
// naive_make_heap 0.016'008'112 sec
// 
// std:: sort_heap 0.131'853'032 sec
// naive_sort_heap 0.133'713'228 sec
// 
// random
// 
// merge sort      0.110'125'566 sec
// heap sort       0.118'919'108 sec
// heap sort naive 0.120'457'023 sec
// 
// few unique
// 
// merge sort      0.057'346'665 sec
// heap sort       0.065'191'019 sec
// heap sort naive 0.064'737'361 sec
// 
// sorted
// 
// merge sort      0.016'284'412 sec
// heap sort       0.061'801'118 sec
// heap sort naive 0.067'460'120 sec
// 
// reversed
// 
// merge sort      0.038'613'742 sec
// heap sort       0.065'233'715 sec
// heap sort naive 0.062'140'641 sec
// 
// nearly sorted
// 
// merge sort      0.067'201'065 sec
// heap sort       0.091'678'384 sec
// heap sort naive 0.097'427'297 sec