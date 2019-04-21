// Sean Parent - C++ Seasoning [GN2013]
// Sean Parent - Better Code: Data Structures [CppCon 2015]

template <
typename I, // random iterator
typename P  // unary predicate
>
auto test_stable_partition (I f, I l, P p) -> I
{
    auto n =  l - f;
    if  (n == 0) return f;
    if  (n == 1) return f + p (*f);
    
    auto m = f + (n / 2);

    return std::rotate
    (
        test_stable_partition (f, m, p), m,
        test_stable_partition (m, l, p)
    );
} 

TEST_OFF
{
    int a [] = { 21, 22, 11, 23, 24, 25, 12, 13, 14, 26, 27, 15, 16, 17 };

    test_stable_partition (std::begin(a), std::end(a), [&](int i) { return i < 20; });

    for (const int i : a) cout << i << " "; cout << endl;
};

template <
typename I, // random iterator
typename P  // unary predicate
>
auto test_stable_partition_position (I f, I l, P p) -> I
{
    auto n = l - f;
    if  (n == 0) return f;
    if  (n == 1) return f + p (f); // no *
    
    auto m = f + (n / 2);

    return std::rotate
    (
        test_stable_partition_position (f, m, p), m,
        test_stable_partition_position (m, l, p)
    );
} 

TEST_OFF
{
    using namespace std;

    vector<int > a { 1, 2, 3, 4, 5, 5, 4, 3, 2, 1 };
    vector<bool> b { 0, 1, 0, 1, 0, 0, 1, 0, 1, 0 };

    auto p = test_stable_partition_position (begin(a), end(a), [&](auto i) {
        return *(begin(b) + (i - begin(a)));
    });

    for (auto f = begin(a); f != p; ++f) cout << *f << " "; cout << "^ ";
    for (auto f = p;   f != end(a); ++f) cout << *f << " "; cout << endl;
};

// template <
// typename I, // I models BidirectionalIterator
// typename S  // S models UnaryPredicate
// >
template <class I, class S> auto gather (I f, I l, I p, S s) -> std::pair<I,I>
{
    return { std::stable_partition (f, p, std::not_fn (s)),
             std::stable_partition (p, l, s) };
}

TEST_OFF
{
    using namespace std;

    vector<int> a { 22, 11, 23, 24, 25, 12, 13, 14, 26, 27, 15, 16, 17 };

    const auto p = gather (std::begin(a), std::end(a), std::begin(a) + 6, [&](int i) { return i < 20; });

    for (auto f = begin(a); f != p.first;  ++f) cout << *f << " "; cout << "[ ";
    for (auto f = p.first;  f != p.second; ++f) cout << *f << " "; cout << "] ";
    for (auto f = p.second; f != end(a);   ++f) cout << *f << " "; cout << endl;
};

TEST_OFF
{
    std::vector<int> v1 (1'000'000);
    std::iota    (v1.begin(), v1.end(), 0);
    std::shuffle (v1.begin(), v1.end(), std::mt19937{std::random_device{}()});
    std::vector<int> v2 = v1;
    std::vector<int> v3 = v1;

    // Complexity
    // Given N = last - first
    // 1) Exactly N applications of the predicate and O(N) swaps if there is enough extra memory. If memory is insufficient, at most N log N swaps.
    // 2) O(N log N) swaps and O(N) applications of the predicate
    // https://en.cppreference.com/w/cpp/algorithm/stable_partition

    auto e = std::execution::par;//_unseq;

    Time t0; std::stable_partition (   v1.begin(), v1.end(), [&](int i) noexcept { return i < v1.size () / 2; });
    Time t1; std::stable_partition (e, v2.begin(), v2.end(), [&](int i) noexcept { return i < v2.size () / 2; });
    Time t2; test_stable_partition (   v3.begin(), v3.end(), [&](int i) noexcept { return i < v3.size () / 2; });
    Time t3;

    cout << "std::stable_partition " <<  t1-t0 << " sec" << endl;
    cout << "par::stable_partition " <<  t2-t1 << " sec" << endl;
    cout << "test_stable_partition " <<  t3-t2 << " sec" << endl;

    assert (v1 == v2);
};

// Sean Parent, "Generic Programming", Pacific++ talk, 2018
// Elements of Programming, Alexander Stepanov, Paul McJones
template <typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
        UnaryPredicate(P) && ValueType(I) == Domain(P))
I partition_semistable(I f, I l, P p) {
    // Precondition: mutable_bounded_range(f, l)
    I i = find_if(f, l, p);
    if (i == l) return i;
    I j = successor(i);
    while (true) {
        j = find_if_not(j, l, p);
        if (j == l) return i;
        swap_step(i, j);
    }
}
