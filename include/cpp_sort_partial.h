// Sean Parent - C++ Seasoning [GN2013]
// Sean Parent - Better Code: Data Structures [CppCon 2015]

template <
typename I // I models RandomAccessIterator
>
void sort_subrange (I f, I l, I sf, I sl)
{
    if (sf == sl) return;
    if (sf != f ) { nth_element (f, sf, l); ++sf; }
    partial_sort (sf, sl, l);
} 

TEST_OFF
{
    const int nn = 5;
    std::vector<std::string> v;
    for (int i=0; i<nn; i++)
    for (int j=0; j<26; j++) v.emplace_back (std::string (1, '0' + i) + std::string (1, 'a' + j) + " ");
    std::shuffle (v.begin(), v.end(), std::mt19937{std::random_device{}()});

    for (int64_t i=0; i<nn; i++) {
    for (int64_t j=0; j<26; j++) {
        cout << v[i*26+ j];
    };  cout << endl;
    };  cout << endl;

    nth_element   (v.begin(), v.begin() + 26*(nn/2), v.end());

    for (int64_t i=0; i<nn; i++) {
    for (int64_t j=0; j<26; j++) {
        cout << v[i*26+ j];
    };  cout << endl;
    };  cout << endl;

    sort_subrange (v.begin(), v.end(), v.begin() + 26*(nn/2), v.begin() + 26*(nn/2) + 26);

    for (int64_t i=0; i<nn; i++) {
    for (int64_t j=0; j<26; j++) {
        cout << v[i*26+ j];
    };  cout << endl;
    };  cout << endl;

    partial_sort  (v.begin() + 26*(nn/2) + 26, v.begin() + 26*(nn/2) + 26 + 26, v.end());

    for (int64_t i=0; i<nn; i++) {
    for (int64_t j=0; j<26; j++) {
        cout << v[i*26+ j];
    };  cout << endl;
    };  cout << endl;
};

template <class I> auto slide (I f, I l, I p) -> std::pair<I,I>
{
    if (p < f) return { p, std::rotate (p, f, l) };
    if (l < p) return { std::rotate (f, l, p), p };
    return {f, l};
}

TEST_ON
{
};
