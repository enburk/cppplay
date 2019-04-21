template <typename I> // BidirectionalIterator 

auto rotate_by_reverse (I f, I m, I l) -> I // predated 1971
{
    // AB => (AB)' => B'A' => (B')'(A')' =>
    // BA

    std::reverse (f, m);
    std::reverse (m, l);
    std::reverse (f, l);

    m = f + (l - m);

    return m;
}

// CppCon 2015: Sean Parent "Better Code: Data Structures"
// 1. rotate realisation by reverse
// 2. reverse realisation by rotate - n log n for forward iterators
// 3. forward iterational rotate might be faster then bidirectional and even random (because of cache)

// Gries-Mills, 1981
// Alexander A. Stepanov. “From Mathematics to Generic Programming”
/// template <ForwardIterator I>
/// I rotate_(I f, I m, I l, std::forward_iterator_tag) {
///     if (f == m) return l;
///     if (m == l) return f;
///     pair<I, I> p = swap_ranges(f, m, m, l);
///     while (p.first != m || p.second != l) {
///         if (p.second == l) {
///             rotate_unguarded(p.first, m, l);
///             return p.first;
///         }
///         f = m;
///         m = p.second;
///         p = swap_ranges(f, m, m, l);
///     }
///     return m;
/// }

using std::pair;

template <typename I> // ForwardIterator

auto swap_ranges(I f1, I l1, I f2, I l2) -> pair<I, I>
{
    while (f1 != l1 && f2 != l2) std::iter_swap (f1++, f2++); return {f1, f2};
}

template <typename I> // ForwardIterator

void rotate_unguarded (I f, I m, I l)
{
    pair<I, I> p = swap_ranges(f, m, m, l);
    while (p.first != m || p.second != l) {
        f = p.first; if (m == f) m = p.second;
        p = swap_ranges(f, m, m, l);
    }
}

template <typename I> // ForwardIterator

auto gries_mills_rotate (I f, I m, I l) -> I
{
    if (f == m) return l;
    if (m == l) return f;
    pair<I, I> p = swap_ranges(f, m, m, l);
    while (p.first != m || p.second != l) {
        if (p.second == l) {
            rotate_unguarded(p.first, m, l);
            return p.first;
        }
        f = m;
        m = p.second;
        p = swap_ranges(f, m, m, l);
    }
    return m;
}

TEST_OFF
{
    std::vector<char> v0 (10'000'000);

    std::iota (v0.begin (), v0.end (), 0); auto v1 = v0, v2 = v0, v3 = v0; 

    Time t0; rotate_by_reverse  (v1.begin(), v1.begin()+v0.size()/3, v1.end());
    Time t1; gries_mills_rotate (v2.begin(), v2.begin()+v0.size()/3, v2.end());
    Time t2; std::rotate        (v3.begin(), v3.begin()+v0.size()/3, v3.end());
    Time t3;

    cout << "rotate_by_reverse  " << t1-t0 << " sec" << endl;
    cout << "gries_mills_rotate " << t2-t1 << " sec" << endl;
    cout << "std::rotate        " << t3-t2 << " sec" << endl;
    cout <<  endl;

    assert (v1 == v3);
    assert (v2 == v3);
};

// Possible output:
// 
// rotate_by_reverse  0.004'137'098 sec
// gries_mills_rotate 0.012'888'354 sec
// std::rotate        0.004'358'795 sec