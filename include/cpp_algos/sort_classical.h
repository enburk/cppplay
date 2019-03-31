template <
typename I, // forward iterator
typename C = std::less<>
>
void selection_sort (I f, I l, C compare = C {}) // O (N**2), minimizes the number of swaps
{
    for (auto i=f; i!=l; ++i) std::iter_swap (std::min_element (i, l, compare), i); 
}

template <
typename I,
typename C = std::less<>
>
void insertion_sort (I f, I l, C compare = C {}) // O (N**2), better on nearly sorted input; stable
{
    for (auto i=f; i!=l; ++i)
        std::rotate (std::upper_bound (f, i, *i, compare), i, std::next (i));
}

template <
typename I,
typename C = std::less<>
>
void insertion_sort_linear (I f, I l, C compare = C {}) // O (N) for nearly sorted input
{
    for (auto i = std::next (f); i != l; ++i)
    {
        auto const insertion = std::find_if_not (
            std::reverse_iterator<I>(i),
            std::reverse_iterator<I>(f),
            [=](auto const & e){ return compare (*i, e); }).base ();
        std::rotate(insertion, i, std::next (i)); 
    }
}

template <
typename I,
typename C = std::less<>
>
void merge_sort (I f, I l, C compare = C {}) // O (N log N), stable
{
    auto const N = std::distance (f, l); if (N <= 1) return;                   
    auto const middle = std::next (f, N / 2);
    merge_sort (f, middle, compare);
    merge_sort (middle, l, compare);
    std::inplace_merge (f, middle, l, compare); // will allocate and deallocate its temporary buffer
}

// https://stackoverflow.com/questions/24650626/how-to-implement-classic-sorting-algorithms-in-modern-c

TEST_OFF
{
    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back (Test::Function (random, std::tuple{10'000},       "random       "));
    tests.emplace_back (Test::Function (random, std::tuple{10'000, 0, 5}, "few unique   "));
    tests.emplace_back (Test::Function (sorted, std::tuple{10'000},       "sorted       "));
    tests.emplace_back (Test::Function (sorted, std::tuple{10'000, -1.0}, "reversed     "));
    tests.emplace_back (Test::Function (sorted, std::tuple{10'000,  0.8}, "nearly sorted"));

    for (auto test : tests)
    {
        cout << test.description () << endl << endl;

        const auto v0 = test.sample_data (); auto v1 = v0, v2 = v0, v3 = v0, v4 = v0; 

        const Time t0; selection_sort        (v1.begin (), v1.end ());
        const Time t1; insertion_sort        (v2.begin (), v2.end ());
        const Time t2; insertion_sort_linear (v3.begin (), v3.end ());
        const Time t3; merge_sort            (v4.begin (), v4.end ());
        const Time t4;

        cout << "selection sort " << t1-t0 << " sec" << endl;
        cout << "insertion sort " << t2-t1 << " sec" << endl;
        cout << "insertion sort " << t3-t2 << " sec" << " (linear)" << endl;
        cout << "merge sort     " << t4-t3 << " sec" << endl;
        cout <<  endl;

        assert (std::is_sorted (v1.begin (), v1.end ()));
        assert (std::is_sorted (v2.begin (), v2.end ()));
        assert (std::is_sorted (v3.begin (), v3.end ()));
        assert (std::is_sorted (v4.begin (), v4.end ()));
    }
};

// Possible output:
// 
// random
// 
// selection sort 0.158'999'217 sec
// insertion sort 0.005'508'724 sec
// insertion sort 0.014'025'938 sec (linear)
// merge sort     0.000'715'994 sec
// 
// few unique
// 
// selection sort 0.139'366'025 sec
// insertion sort 0.004'331'685 sec
// insertion sort 0.011'691'565 sec (linear)
// merge sort     0.000'459'813 sec
// 
// sorted
// 
// selection sort 0.142'359'735 sec
// insertion sort 0.000'153'134 sec
// insertion sort 0.000'024'223 sec (linear)
// merge sort     0.000'156'829 sec
// 
// reversed
// 
// selection sort 0.137'865'474 sec
// insertion sort 0.010'138'055 sec
// insertion sort 0.026'257'373 sec (linear)
// merge sort     0.000'267'676 sec
// 
// nearly sorted
// 
// selection sort 0.138'670'558 sec
// insertion sort 0.002'389'796 sec
// insertion sort 0.005'569'895 sec (linear)
// merge sort     0.000'499'225 sec
