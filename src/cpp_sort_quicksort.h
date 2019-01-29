template <class type> void quicksort (std::vector<type> & v, int l, int u) // Jon Bentley, Programming Pearls
{
    if (l >= u) return;
    std::swap (v [l], v [randint (l, u)]);
    int m = l; for (int i = l+1; i <= u; i++) if (v [i] < v [l]) std::swap (v [++m], v [i]);
    std::swap (v [l], v [m]);
    quicksort (v, l, m-1);
    quicksort (v, m+1, u);
}
template <class type> void quicksort (std::vector<type> & v) { quicksort (v, 0, v.size () - 1); } 

TEST_ON
{
    {
        std::vector<int> v0; for (int i=0; i<25; i++) v0.push_back (randint (10, 99));
        std::vector<int> v1 = v0, v2 = v0;

        Time t0; quicksort (v1);
        Time t1; std::sort (v2.begin (), v2.end ());
        Time t2;

        for (int i : v0) cout << i << " ";  cout << endl;
        for (int i : v1) cout << i << " ";  cout << endl;
        for (int i : v2) cout << i << " ";  cout << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "std::sort: " << t2-t1 << " sec" << endl;
        cout << endl;
    }
    {
        std::vector<int> v0; for (int i=0; i<1'000'000; i++) v0.push_back (randint (0, v0.size ()));
        std::vector<int> v1 = v0, v2 = v0, v3 = v0;

        Time t0; quicksort (v1);
        Time t1; std::sort (v2.begin (), v2.end ());
        Time t2; std::sort (std::execution::par_unseq, v3.begin (), v3.end ());
        Time t3;

        cout << "1'000'000 random ints" << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "std::sort: " << t2-t1 << " sec" << endl;
        cout << "par_unseq: " << t3-t2 << " sec" << endl;
        cout << endl;
    }
    {
        std::vector<int> v0; for (int i=0; i<250'000; i++) v0.push_back (randint (10, 99));
        std::vector<int> v1 = v0, v2 = v0, v3 = v0, v4 = v0;

        Time t0; quicksort (v1);
        Time t1; std::sort (v2.begin (), v2.end ());
        Time t2; std::sort (std::execution::par_unseq, v3.begin (), v3.end ());
        Time t3; counting  (v4.begin (), v4.end ());
        Time t4;

        cout << "quicksort pitfall on many duplicates" << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "std::sort: " << t2-t1 << " sec" << endl;
        cout << "par_unseq: " << t3-t2 << " sec" << endl;
        cout << "counting : " << t4-t3 << " sec" << endl;
        cout << endl;
    }
    {
        std::vector<int> v0 (1'000'000); std::iota (v0.begin (), v0.end (), 0);
        int nn =   (int) v0.size (); int n = nn / 100; while (n--) std::swap (v0 [randint (0, nn)], v0 [randint (0, nn)]);
        std::vector<int> v1 = v0, v2 = v0, v3 = v0;

        Time t0; quicksort (v1);
        Time t1; std::sort (v2.begin (), v2.end ());
        Time t2; std::sort (std::execution::par_unseq, v3.begin (), v3.end ());
        Time t3;

        cout << "std::sort advantage on partially sorted array" << endl;
        cout << "quicksort: " << t1-t0 << " sec" << endl;
        cout << "std::sort: " << t2-t1 << " sec" << endl;
        cout << "par_unseq: " << t3-t2 << " sec" << endl;
        cout << endl;
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