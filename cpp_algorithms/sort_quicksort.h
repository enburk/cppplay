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
    auto const pivot = *std::next(first, N / 2); // deterministic
    auto middle1 = std::partition(first, last, [pivot](const auto& em){ return em < pivot; });
    auto middle2 = std::partition(middle1, last, [pivot](const auto& em){ return !(pivot < em); });
    quicksort2(first, middle1, compare); 
    quicksort2(middle2,  last, compare); 
}

template <
typename I,
typename C = std::less<>
>
void quicksort3 (I first, I last, C compare = C {})
{
    auto const N = std::distance(first, last); if (N <= 1) return; 
    auto const pivot = std::next(first, N / 2); // median
    std::nth_element(first, pivot, last, compare);
    quicksort3(first, pivot, compare); 
    quicksort3(pivot,  last, compare); 
}

TEST_OFF
{
    auto organ_pipes = [] (int n)
    {
        std::vector<int> v1 = sorted (n/2);
        std::vector<int> v2 = sorted (n-n/2, -1);
        v1.insert (v1.end(), v2.begin(), v2.end()); return v1;
    };

    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back ("random       ", random,    1'000'000);
    tests.emplace_back ("few unique   ", random,      250'000, 0,99);
    tests.emplace_back ("sorted       ", sorted,    1'000'000);
    tests.emplace_back ("reversed     ", sorted,    1'000'000, -1.0);
    tests.emplace_back ("nearly sorted", sorted,    1'000'000,  0.8);
    tests.emplace_back ("organ pipes  ", organ_pipes, 250'000);

    for (const Test & test : tests)
    {
        cout << test.description () << endl << endl;

        auto v0 = test.sample_data ();
        auto v1 = v0, v2 = v0, v3 = v0, v4 = v0, v5 = v0; 

        Time t0; quicksort  (v1);
        Time t1; quicksort1 (v2.begin (), v2.end ());
        Time t2; quicksort2 (v3.begin (), v3.end ());
        Time t3; quicksort3 (v4.begin (), v4.end ());
        Time t4; std::sort  (v5.begin (), v5.end ());
        Time t5;

        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "quicksort: " << t2-t1 << " sec (modern)" << endl;
        cout << "quicksort: " << t3-t2 << " sec (deterministic)" << endl;
        cout << "quicksort: " << t4-t3 << " sec (median)" << endl;
        cout << "std::sort: " << t5-t4 << " sec" << endl;
        cout << endl;

        assert (std::is_sorted (v1.begin(), v1.end()));
        assert (std::is_sorted (v2.begin(), v2.end()));
        assert (std::is_sorted (v3.begin(), v3.end()));
        assert (std::is_sorted (v4.begin(), v4.end()));
        assert (std::is_sorted (v5.begin(), v5.end()));
    }
};

// Possible output:
// 
// random
// 
// quicksort: 0.105'935'482 sec
// quicksort: 0.117'005'471 sec (modern)
// quicksort: 0.082'582'751 sec (deterministic)
// quicksort: 0.105'585'696 sec (median)
// std::sort: 0.082'564'687 sec
// 
// few unique
// 
// quicksort: 0.205'736'572 sec
// quicksort: 0.005'445'084 sec (modern)
// quicksort: 0.005'690'181 sec (deterministic)
// quicksort: 0.010'876'621 sec (median)
// std::sort: 0.006'331'865 sec
// 
// sorted
// 
// quicksort: 0.047'344'685 sec
// quicksort: 0.060'992'087 sec (modern)
// quicksort: 0.015'923'884 sec (deterministic)
// quicksort: 0.021'907'607 sec (median)
// std::sort: 0.011'404'583 sec
// 
// reversed
// 
// quicksort: 0.050'662'315 sec
// quicksort: 0.062'636'738 sec (modern)
// quicksort: 0.016'007'636 sec (deterministic)
// quicksort: 0.032'087'529 sec (median)
// std::sort: 0.013'980'355 sec
// 
// nearly sorted
// 
// quicksort: 0.076'498'445 sec
// quicksort: 0.085'507'488 sec (modern)
// quicksort: 0.045'291'539 sec (deterministic)
// quicksort: 0.082'542'928 sec (median)
// std::sort: 0.061'732'714 sec
// 
// organ pipes
// 
// quicksort: 0.011'699'356 sec
// quicksort: 0.014'114'194 sec (modern)
// quicksort: 0.230'671'552 sec (deterministic)
// quicksort: 0.012'643'615 sec (median)
// std::sort: 0.007'100'409 sec
