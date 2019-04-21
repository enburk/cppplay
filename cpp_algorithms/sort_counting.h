// really fast when the variation in integer keys is not significantly greater than the number of items

template<typename I> void counting_sort (I first, I last) // I = ForwardIterator
{
    if (first == last || std::next (first) == last) return;

    const auto mmx = std::minmax_element (first, last); // O(N), better to know them aforehead
    const auto min = *mmx.first;
    const auto max = *mmx.second;
    if (min == max) return;

    #pragma warning (push)
    #pragma warning (disable: 26451) // Arithmetic overflow
    #pragma warning (disable: 26446) // Prefer to use gsl::at() instead of unchecked subscript operator

    using integer = typename std::iterator_traits<I>::difference_type;

    std::vector<integer> counts (max - min + 1, 0);

    for (auto it = first; it != last ; ++it) ++counts [*it - min];

    auto value = min;

    for (auto count: counts) first = std::fill_n (first, count, value++);

    #pragma warning (pop)
}

// https://stackoverflow.com/questions/24650626/how-to-implement-classic-sorting-algorithms-in-modern-c

TEST_OFF
{
    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back("         10 keys", random, 1'000'000, 1,          10);
    tests.emplace_back("        100 keys", random, 1'000'000, 1,         100);
    tests.emplace_back("      1'000 keys", random, 1'000'000, 1,       1'000);
    tests.emplace_back("     10'000 keys", random, 1'000'000, 1,      10'000);
    tests.emplace_back("    100'000 keys", random, 1'000'000, 1,     100'000);
    tests.emplace_back("  1'000'000 keys", random, 1'000'000, 1,   1'000'000);
    tests.emplace_back("          sorted", sorted, 1'000'000                );
    tests.emplace_back("        reversed", sorted, 1'000'000,-1.0           );
    tests.emplace_back("   nearly sorted", sorted, 1'000'000, 0.8           );
    tests.emplace_back(" 10'000'000 keys", random, 1'000'000, 1,  10'000'000);
    tests.emplace_back("100'000'000 keys", random, 1'000'000, 1, 100'000'000);

    for (auto test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data (); auto v1 = v0, v2 = v0; 

        Time t0; merge_sort    (v1.begin(), v1.end());
        Time t1; counting_sort (v2.begin(), v2.end());
        Time t2;

        cout << "merge sort    " << t1-t0 << " sec" << endl;
        cout << "counting sort " << t2-t1 << " sec" << endl;
        cout <<  endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
    }
};

// Possible output:
// 
//          10 keys
// 
// merge sort    0.065'327'105 sec
// counting sort 0.004'293'504 sec
// 
//         100 keys
// 
// merge sort    0.068'914'468 sec
// counting sort 0.003'854'629 sec
// 
//       1'000 keys
// 
// merge sort    0.080'462'342 sec
// counting sort 0.003'708'884 sec
// 
//      10'000 keys
// 
// merge sort    0.095'483'857 sec
// counting sort 0.004'516'020 sec
// 
//     100'000 keys
// 
// merge sort    0.106'917'598 sec
// counting sort 0.006'511'280 sec
// 
//   1'000'000 keys
// 
// merge sort    0.109'050'803 sec
// counting sort 0.026'080'016 sec
// 
//           sorted
// 
// merge sort    0.018'154'811 sec
// counting sort 0.016'065'126 sec
// 
//         reversed
// 
// merge sort    0.038'082'366 sec
// counting sort 0.015'462'853 sec
// 
//    nearly sorted
// 
// merge sort    0.065'280'303 sec
// counting sort 0.021'220'779 sec
// 
//  10'000'000 keys
// 
// merge sort    0.108'918'606 sec
// counting sort 0.082'811'494 sec
// 
// 100'000'000 keys
// 
// merge sort    0.108'499'438 sec
// counting sort 0.551'911'857 sec
