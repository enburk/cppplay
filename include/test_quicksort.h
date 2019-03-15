#pragma once

#include "test_aux.h"
#include "test_aux_chrono.h"
#include "test_inheritance.h"
#include <execution> // std::execution::par_unseq

template <class T> struct quicksort_test
{
    quicksort_test (std::vector<T> & v)
    {
        if (v.size () > static_cast<size_t>(std::numeric_limits<int>::max ()))
            throw std::logic_error ("quicksort_test: vector too long");
        x = v.data ();
        quicksort (0, static_cast<int>(v.size ()) - 1);
    }

private:
    T* x;
    void swap (int i, int j) { std::swap (x [i], x [j]); }
    
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
};

template <class T> void quicksort (std::vector<T> & v) { quicksort_test test (v); }

TEST_ON
{
    std::vector<Test> tests;

    tests.emplace_back (TestFunction (random, std::make_tuple (1'000'000),
        "1'000'000 random ints"));

    tests.emplace_back (TestFunction (random, std::make_tuple (250'000, 0, 99),
        "quicksort pitfall on many duplicates"));

    tests.emplace_back (TestFunction (nearly_sorted, std::make_tuple (1'000'000),
        "std::sort advantage on nearly sorted array"));

    for (const Test & test : tests)
    {
        std::vector<int> v1 = test.sample ();
        std::vector<int> v2 = v1;
        std::vector<int> v3 = v1;

        Time t0; quicksort (v1);
        Time t1; std::sort (v2.begin (), v2.end ());
        Time t2; std::sort (std::execution::par_unseq, v3.begin (), v3.end ());
        Time t3;

        cout << test.description () << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "std::sort: " << t2-t1 << " sec" << endl;
        cout << "par_unseq: " << t3-t2 << " sec" << endl;
        cout << endl;

        if (v1 != v2) throw std::logic_error ("quicksort probably wrong");
    }
};

// Possible output:
// 
// 1'000'000 random ints
// quicksort: 0.102'948'499 sec
// std::sort: 0.084'513'345 sec
// par_unseq: 0.019'079'302 sec
// 
// quicksort pitfall on many duplicates
// quicksort: 0.220'040'718 sec
// std::sort: 0.006'493'605 sec
// par_unseq: 0.002'985'901 sec
// 
// std::sort advantage on nearly sorted array
// quicksort: 0.052'452'170 sec
// std::sort: 0.011'930'055 sec
// par_unseq: 0.003'692'860 sec