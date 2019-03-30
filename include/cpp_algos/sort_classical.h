template<class I, class C = std::less<>>
void
selection_sort (I f, I l, C compare = C {}) // O (N²), minimizing the number of swaps
{
    for (auto i=f; i!=l; ++i)
        std::iter_swap (std::min_element (i, l, compare), i); 
}

template<class I, class C = std::less<>>
void
insertion_sort (I f, I l, C compare = C {}) // O (N²), better on nearly sorted input; stable
{
    for (auto i=f; i!=l; ++i)
        std::rotate (std::upper_bound (f, i, *i, compare), i, std::next (i));
}

template<class I, class C = std::less<>>
void
insertion_sort_linear (I f, I l, C compare = C {}) // O (N) for nearly sorted input
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

template<class I, class C = std::less<>>
void
merge_sort (I f, I l, C compare = C {}) // O (N log N), stable
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
    auto vv = inputs (0, 10'000); for (const auto & input : vv)
    {
        cout << input.first << endl << endl;

        const auto & v0 = input.second; auto v1 = v0, v2 = v0, v3 = v0, v4 = v0; 

        Time t0; selection_sort        (v1.begin (), v1.end ());
        Time t1; insertion_sort        (v2.begin (), v2.end ());
        Time t2; insertion_sort_linear (v3.begin (), v3.end ());
        Time t3; merge_sort            (v4.begin (), v4.end ());
        Time t4;

        cout << "selection_sort        " << t1-t0 << " sec" << endl;
        cout << "insertion_sort        " << t2-t1 << " sec" << endl;
        cout << "insertion_sort_linear " << t3-t2 << " sec" << endl;
        cout << "merge_sort            " << t4-t3 << " sec" << endl;
        cout << endl;

        assert (std::is_sorted (v1.begin (), v1.end ()));
        assert (std::is_sorted (v2.begin (), v2.end ()));
        assert (std::is_sorted (v3.begin (), v3.end ()));
        assert (std::is_sorted (v4.begin (), v4.end ()));
    }
};

// Possible output:
// 
// randomized
// 
// selection_sort        0.140'027'540 sec
// insertion_sort        0.006'016'558 sec
// insertion_sort_linear 0.013'680'228 sec
// merge_sort            0.000'840'389 sec
// 
// few_unique
// 
// selection_sort        0.139'673'238 sec
// insertion_sort        0.005'900'784 sec
// insertion_sort_linear 0.012'719'140 sec
// merge_sort            0.000'595'703 sec
// 
// sorted
// 
// selection_sort        0.137'635'286 sec
// insertion_sort        0.000'170'787 sec
// insertion_sort_linear 0.000'010'264 sec
// merge_sort            0.000'181'461 sec
// 
// reversed
// 
// selection_sort        0.137'787'598 sec
// insertion_sort        0.009'672'063 sec
// insertion_sort_linear 0.025'868'532 sec
// merge_sort            0.000'345'680 sec
// 
// nearly_sorted
// 
// selection_sort        0.138'425'587 sec
// insertion_sort        0.000'292'309 sec
// insertion_sort_linear 0.000'153'544 sec
// merge_sort            0.000'229'496 sec
