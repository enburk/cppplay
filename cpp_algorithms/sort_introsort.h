std::sort Introsort
GCC - https://github.com/gcc-mirror/gcc
LLVM's libcxx - https://github.com/llvmmirror/libcxx

template<typename RandIt, typename _Compare>
  inline void
  __sort(RandIt first, RandIt last,
        _Compare comp) {
    if (first != last){
        std::__introsort_loop(first, last,
        std::__lg(last-first)*2,
        comp);
        std::__final_insertion_sort(first, last, comp);
    }
}

template<typename RandIt>
  inline void
    nth_element(RandIt first, RandIt nth,
                RandIt last)
{
    if (first == last || nth == last)
        return;
    std::__introselect(first, nth, last,
    std::__lg(last - first) * 2,
    __gnu_cxx::__ops::__iter_less_iter());
}

template<typename RandIt, typename _Compare>
  inline void
  __partial_sort(RandIt first,
                 RandIt middle,
                 RandIt last,
                _Compare comp)
{
    std::__heap_select(first, middle, last, comp);
    std::__sort_heap(first, middle, comp);
}


  TEST_ON
{
    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back ("random       ", random, 1'000'000);
    tests.emplace_back ("few unique   ", random,   250'000, 0,99); // quicksort pitfall
    tests.emplace_back ("sorted       ", sorted, 1'000'000);
    tests.emplace_back ("reversed     ", sorted, 1'000'000, -1.0);
    tests.emplace_back ("nearly sorted", sorted, 1'000'000,  0.8); // std::sort advantage

    for (const Test & test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data (); auto v1 = v0, v2 = v0, v3 = v0, v4 = v0; 

        Time t0; quicksort (v1);
        Time t1; quicksort (v2.begin (), v2.end ());
        Time t2; std::sort (v3.begin (), v3.end ());
        Time t3; std::sort (std::execution::par_unseq, v4.begin (), v4.end ());
        Time t4;

        cout << test.description () << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "quicksort: " << t2-t1 << " sec" << endl;
        cout << "std::sort: " << t3-t2 << " sec" << endl;
        cout << "par_unseq: " << t4-t3 << " sec" << endl;
        cout << endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
        assert (std::is_sorted (v3.begin(), v3.end()));
        assert (std::is_sorted (v4.begin(), v4.end()));
    }
};
