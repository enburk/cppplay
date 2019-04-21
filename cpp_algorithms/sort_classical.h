// https://en.wikipedia.org/wiki/Bubble_sort
// https://stackoverflow.com/questions/24650626/how-to-implement-classic-sorting-algorithms-in-modern-c
// https://stackoverflow.com/questions/17270628/insertion-sort-vs-bubble-sort-algorithms
// In insertion sort elements are bubbled into the sorted section,
// while in bubble sort the maximums are bubbled out of the unsorted section.

// In bubble sort in ith iteration you have n-i-1 inner iterations (n^2)/2 total,
// but in insertion sort you have maximum i iterations on i'th step, but i/2 on average,
// as you can stop inner loop earlier, after you found correct position for the current element.
// So you have (sum from 0 to n) / 2 which is (n^2) / 4 total;

template <
typename I, // bidirectional iterator
typename C = std::less<>
>
void bubble_sort (I f, I l, C compare = C {}) // O (N**2)
{
    while (f != l)
    {
        for (auto i=f, j=std::next(f); j!=l; ++i, ++j)
            if (! compare(*i, *j))
                std::swap(*i, *j);
        --l;
    }
} 

template <
typename I, // forward iterator
typename C = std::less<>
>
void bubble_sort_opt (I f, I l, C compare = C {}) // O (N**2)
{ 
    while (f != l)
    {
        auto nextl = f;
        for (auto i=f, j=std::next(f); j!=l; ++i, ++j)
            if (! compare(*i, *j)) {
                std::swap(*i, *j); nextl = j; }
        l = nextl;
    }
} 

// https://commons.wikimedia.org/wiki/File:Bubble_sort_animation.gif

template <
typename I,
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

TEST_OFF
{
    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back ("random       ", random, 10'000);
    tests.emplace_back ("few unique   ", random, 10'000, 0, 5);
    tests.emplace_back ("sorted       ", sorted, 10'000);
    tests.emplace_back ("reversed     ", sorted, 10'000, -1.0);
    tests.emplace_back ("nearly sorted", sorted, 10'000,  0.8);

    for (auto test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data (); auto v1 = v0, v2 = v0, v3 = v0, v4 = v0, v5 = v0, v6 = v0; 

        Time t0; selection_sort        (v1.begin(), v1.end());
        Time t1; insertion_sort        (v2.begin(), v2.end());
        Time t2; insertion_sort_linear (v3.begin(), v3.end());
        Time t3; bubble_sort           (v4.begin(), v4.end());
        Time t4; bubble_sort_opt       (v5.begin(), v5.end());
        Time t5; merge_sort            (v6.begin(), v6.end());
        Time t6;

        cout << "selection sort " << t1-t0 << " sec" << endl;
        cout << "insertion sort " << t2-t1 << " sec" << endl;
        cout << "........linear " << t3-t2 << " sec" << endl;
        cout << "bubble sort... " << t4-t3 << " sec" << endl;
        cout << ".....optimized " << t5-t4 << " sec" << endl;
        cout << "merge sort.... " << t6-t5 << " sec" << endl;
        cout <<  endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
        assert (std::is_sorted (v3.begin(), v3.end()));
        assert (std::is_sorted (v4.begin(), v4.end()));
        assert (std::is_sorted (v5.begin(), v5.end()));
        assert (std::is_sorted (v6.begin(), v6.end()));
    }
};

// Possible output:
// 
// selection sort 0.146'152'371 sec
// insertion sort 0.005'510'776 sec
// ........linear 0.014'347'807 sec
// bubble sort... 0.121'494'491 sec
// .....optimized 0.123'193'746 sec
// merge sort.... 0.000'711'068 sec
// 
// few unique
// 
// selection sort 0.138'505'107 sec
// insertion sort 0.004'391'214 sec
// ........linear 0.011'463'712 sec
// bubble sort... 0.113'945'347 sec
// .....optimized 0.120'851'984 sec
// merge sort.... 0.000'445'854 sec
// 
// sorted
// 
// selection sort 0.138'324'466 sec
// insertion sort 0.000'139'176 sec
// ........linear 0.000'025'454 sec
// bubble sort... 0.032'201'276 sec
// .....optimized 0.000'012'727 sec
// merge sort.... 0.000'273'425 sec
// 
// reversed
// 
// selection sort 0.138'848'325 sec
// insertion sort 0.009'647'450 sec
// ........linear 0.026'989'379 sec
// bubble sort... 0.114'859'636 sec
// .....optimized 0.121'343'409 sec
// merge sort.... 0.000'262'750 sec
// 
// nearly sorted
// 
// selection sort 0.138'953'424 sec
// insertion sort 0.003'046'261 sec
// ........linear 0.005'686'491 sec
// bubble sort... 0.068'451'370 sec
// .....optimized 0.072'446'817 sec
// merge sort.... 0.000'507'847 sec

