inline auto random = []
(
    int n,
    int l = std::numeric_limits<int>::min (),
    int u = std::numeric_limits<int>::max ()
)
{
    std::vector<int> v (n);
    for (int & i : v) i = randint (l, u);
    return v;
};

inline auto sorted = [] (int n, double k = 1.0)
{
    std::vector<int> v (n);
    std::iota (v.begin(), v.end(), 0);

    if (k < 0.0) std::reverse (v.begin(), v.end());
    if (k < 0.0) k = -k;
    if (k > 1.0) k = 1.0;

    long c = std::lround (v.size () * (1.0 - k));
    
    while (c-- > 0) std::swap (
        v.at (randint (0, n-1)),
        v.at (randint (0, n-1)));
    return v;
};

TEST_OFF
{
    using Test = Test<std::vector<int>>; std::vector<Test> tests;
    tests.emplace_back (Test::Function (random, std::tuple{5},        "random       "));
    tests.emplace_back (Test::Function (random, std::tuple{30, 0, 5}, "few unique   "));
    tests.emplace_back (Test::Function (sorted, std::tuple{25},       "sorted       "));
    tests.emplace_back (Test::Function (sorted, std::tuple{25, -1.0}, "reversed     "));
    tests.emplace_back (Test::Function (sorted, std::tuple{25,  0.8}, "nearly sorted"));

    for (auto test : tests)
    {
        cout << test.description () << endl;
        cout << test.sample_data () << endl;
        cout << endl;
    }
};

// Possible output:
// 
// random
// 1006811233 1168850893 1205620418 -1570519362 -1789997688
// 
// few unique
// 4 5 0 5 4 0 1 2 4 5 3 3 4 2 1 1 0 3 2 1 0 5 4 5 5 4 4 2 3 0
// 
// sorted
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
// 
// reversed
// 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// 
// nearly sorted
// 0 1 2 4 3 23 6 7 8 9 10 11 12 16 14 15 20 17 18 19 13 21 22 5 24
