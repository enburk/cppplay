#pragma once
#include "aux_aux.h"
#include <execution> // std::execution::par_unseq

namespace sort
{
    auto random        (int f, int n) { std::vector<int> v (n); for (int i=0; i<n; i++) v [i] = f + randint (0, n-1);                   return v; }
    auto few_unique    (int f, int n) { std::vector<int> v (n); for (int i=0; i<n; i++) v [i] = f + randint (0, (int) std::log2 (n-1)); return v; }
    auto sorted        (int f, int n) { std::vector<int> v (n); for (int i=0; i<n; i++) v [i] = f + i;                                  return v; }
    auto reversed      (int f, int n) { std::vector<int> v (n); for (int i=0; i<n; i++) v [i] = f + n-1-i;                              return v; }
    auto nearly_sorted (int f, int n) { std::vector<int> v = sorted (f, n);

        int c = (int) std::log (n-1); while (c-- > 0) std::swap (v [randint (0, n-1)], v [randint (0, n-1)]); return v;
    }
    auto inputs        (int f, int n) 
    {
        std::vector<std::pair<std::string, std::vector<int>>> v;

        v.emplace_back ("random       ", random        (f, n));
        v.emplace_back ("few unique   ", few_unique    (f, n));
        v.emplace_back ("sorted       ", sorted        (f, n));
        v.emplace_back ("reversed     ", reversed      (f, n));
        v.emplace_back ("nearly sorted", nearly_sorted (f, n));

        return v;
    }

    TEST_ON
    {
        auto v = inputs (0, 10); for (const auto & p : v)
        {
            cout << p.first << " : "; for (int i : p.second) cout << i << " "; cout << endl;
        }
    };

    #include "cpp_sort_classical.h"
    #include "cpp_sort_counting.h"
    #include "cpp_sort_heapsort.h"
    #include "cpp_sort_quicksort.h"
    #include "cpp_sort_sleepsort.h"
    #include "cpp_sort_partial.h"
    #include "cpp_sort_stable.h"
}

// Possible output:
// 
// random        : 0 9 4 1 0 6 7 6 0 0
// few unique    : 0 1 0 2 3 3 0 1 2 3
// sorted        : 0 1 2 3 4 5 6 7 8 9
// reversed      : 9 8 7 6 5 4 3 2 1 0
// nearly sorted : 0 1 9 3 4 5 6 2 8 7
