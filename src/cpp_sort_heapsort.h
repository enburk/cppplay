template<class RandomIt, class Compare = std::less<>>
void heap_sort (RandomIt first, RandomIt last, Compare cmp = Compare{})
{
    std::make_heap (first, last, cmp);
    std::sort_heap (first, last, cmp);
}

// NOTE: is O(N log N), not O(N) as std::make_heap
template<class RandomIt, class Compare = std::less<>>
void naive_make_heap (RandomIt first, RandomIt last, Compare cmp = Compare{})
{
    for (auto it = first; it != last;) std::push_heap (first, ++it, cmp); 
}

template<class RandomIt, class Compare = std::less<>>
void naive_sort_heap (RandomIt first, RandomIt last, Compare cmp = Compare{})
{
    for (auto it = last; it != first;) std::pop_heap (first, it--, cmp);
}

// https://stackoverflow.com/questions/24650626/how-to-implement-classic-sorting-algorithms-in-modern-c

TEST_ON
{
    {
        auto v1 = random (0, 1'000'000); auto v2 = v1;

        Time t0; std:: make_heap (v1.begin (), v1.end ());
        Time t1; naive_make_heap (v2.begin (), v2.end ());
        Time t2; std:: sort_heap (v1.begin (), v1.end ());
        Time t3; naive_sort_heap (v2.begin (), v2.end ());
        Time t4;

        cout << "std:: make_heap " <<  t1-t0 << " sec" << endl;
        cout << "naive_make_heap " <<  t2-t1 << " sec" << endl << endl;
        cout << "std:: sort_heap " <<  t3-t2 << " sec" << endl;
        cout << "naive_sort_heap " <<  t4-t3 << " sec" << endl << endl;

        assert (std::is_sorted (v1.begin (), v1.end ()));
        assert (std::is_sorted (v2.begin (), v2.end ()));
    }
    auto vv = inputs (0, 1'000'000); for (auto & input : vv)
    {
        auto & v = input.second;

        Time t0; heap_sort (v.begin (), v.end ());
        Time t1;

        cout << input.first << " " <<  t1-t0 << " sec" << endl;

        assert (std::is_sorted (v.begin (), v.end ()));
    }
};

// Possible output:
// 
// std:: make_heap 0.007'630'416 sec
// naive_make_heap 0.011'381'579 sec
// 
// std:: sort_heap 0.120'595'145 sec
// naive_sort_heap 0.130'495'883 sec
// 
// randomized    0.124'512'578 sec
// few_unique    0.067'768'901 sec
// sorted        0.063'048'028 sec
// reversed      0.067'867'023 sec
// nearly_sorted 0.063'868'300 sec

