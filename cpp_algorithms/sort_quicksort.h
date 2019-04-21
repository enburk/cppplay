template <class T> struct quicksort_test
{
    #pragma warning (push)
    #pragma warning (disable: 26493) // Don't use C-style casts
    #pragma warning (disable: 26481) // Don't use pointer arithmetic. Use span instead
    #pragma warning (disable: 26494) // Variable 'i' is uninitialized. Always initialize an object
    #pragma warning (disable: 26446) // Prefer to use gsl::at() instead of unchecked subscript operator

    quicksort_test (std::vector<T> & v)
    {
        x = v.data (); quicksort (0, (int)v.size ()-1);
    }
    private: T* x; void swap (int i, int j) noexcept { std::swap (x [i], x [j]); }

    // Jon Bentley, Programming Pearls [Addison-Wesley, 2000]
    // Beautiful Code, Edited by Andy Oram and Greg Wilson [2007 O’Reilly]

    void quicksort(int l, int u)
    {   int i, m;
        if (l >= u) return;
        swap(l, randint(l, u));
        m = l;
        for (i = l+1; i <= u; i++)
            if (x[i] < x[l])
                swap(++m, i);
        swap(l, m);
        quicksort(l, m-1);
        quicksort(m+1, u);
    }

    #pragma warning (pop)
};

template <class T> void quicksort (std::vector<T> & v) { quicksort_test test (v); }

template <
typename I,
typename C = std::less<>
>
void quicksort1 (I first, I last, C compare = C {})
{
    auto const N = std::distance(first, last); if (N <= 1) return; 
    auto const pivot = *std::next(first, randint<decltype(N)>(0, N-1));
    auto middle1 = std::partition(first, last, [pivot](const auto& em){ return em < pivot; });
    auto middle2 = std::partition(middle1, last, [pivot](const auto& em){ return !(pivot < em); });
    quicksort1(first, middle1, compare); 
    quicksort1(middle2,  last, compare); 
}

template <
typename I,
typename C = std::less<>
>
void quicksort2 (I first, I last, C compare = C {})
{
    auto const N = std::distance(first, last); if (N <= 1) return; 
    auto const pivot = std::next(first, N / 2); // deterministic
    std::nth_element(first, pivot, last, compare);
    quicksort2(first, pivot, compare); 
    quicksort2(pivot,  last, compare); 
}

TEST_ON
{
    auto organ_pipes = [] (int n)
    {
        std::vector<int> v1 = sorted (n/2);
        std::vector<int> v2 = sorted (n-n/2, -1);
        v1.insert (v1.end(), v2.begin(), v2.end()); return v1;
    };

    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back ("random       ", random,    1'000'000);
    tests.emplace_back ("few unique   ", random,      250'000, 0,99); // quicksort pitfall
    tests.emplace_back ("sorted       ", sorted,    1'000'000);
    tests.emplace_back ("reversed     ", sorted,    1'000'000, -1.0);
    tests.emplace_back ("nearly sorted", sorted,    1'000'000,  0.8); // std::sort advantage
    tests.emplace_back ("organ pipes  ", organ_pipes, 250'000);

    for (const Test & test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data (); auto v1 = v0, v2 = v0, v3 = v0, v4 = v0; 

        Time t0; quicksort  (v1);
        Time t1; quicksort1 (v2.begin (), v2.end ());
        Time t2; quicksort2 (v3.begin (), v3.end ());
        Time t3; std::sort  (v4.begin (), v4.end ());
        Time t4;

        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "quicksort: " << t2-t1 << " sec (nth_element)" << endl;
        cout << "quicksort: " << t3-t2 << " sec (deterministic)" << endl;
        cout << "std::sort: " << t4-t3 << " sec" << endl;
        cout << endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
        assert (std::is_sorted (v3.begin(), v3.end()));
        assert (std::is_sorted (v4.begin(), v4.end()));
    }
};

// Possible output:
// 
// 60 82 21 23 69 37 64 22 23 28 43 68 51 82 34 34 32 64 16 90 57 26 98 94 67
// 16 21 22 23 23 26 28 32 34 34 37 43 51 57 60 64 64 67 68 69 82 82 90 94 98
// 16 21 22 23 23 26 28 32 34 34 37 43 51 57 60 64 64 67 68 69 82 82 90 94 98
// quicksort: 0.000'001'643 sec
// std::sort: 0.000'000'821 sec
// 
// 1'000'000 ints
// quicksort: 0.117'334'995 sec
// std::sort: 0.089'499'536 sec
// par_unseq: 0.018'987'362 sec
// 
// quicksort pitfall on many duplicates
// quicksort: 0.230'974'662 sec
// std::sort: 0.007'090'959 sec
// par_unseq: 0.003'158'744 sec
// 
// std::sort advantage on partially sorted array
// quicksort: 0.060'318'305 sec
// std::sort: 0.016'985'949 sec
// par_unseq: 0.004'460'176 sec