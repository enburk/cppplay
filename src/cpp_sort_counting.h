// really fast when the variation in integer keys is not significantly greater than the number of items

template<typename ForwardIterator> void counting (ForwardIterator first, ForwardIterator last)
{
    if (first == last || std::next (first) == last) return;

    auto mm  = std::minmax_element (first, last);
    auto min = *mm.first;
    auto max = *mm.second;
    if (min == max) return;

    std::vector<std::iterator_traits<ForwardIterator>::difference_type> counts (max - min + 1, 0);

    for (auto it = first ; it != last ; ++it) ++counts [*it - min];

    for (auto count: counts) first = std::fill_n (first, count, min++);
}

// https://stackoverflow.com/questions/24650626/how-to-implement-classic-sorting-algorithms-in-modern-c

TEST_OFF
{
    auto vv = inputs (0, 1'000'000); for (auto & input : vv)
    {
        auto & v = input.second;

        Time t0; counting (v.begin (), v.end ());
        Time t1;

        cout << input.first << " " <<  t1-t0 << " sec" << endl;

        assert (std::is_sorted (v.begin (), v.end ()));
    }
};

// Possible output:
// 
// randomized    0.018'867'893 sec
// few_unique    0.003'718'319 sec
// sorted        0.015'132'742 sec
// reversed      0.013'332'085 sec
// nearly_sorted 0.014'917'205 sec

