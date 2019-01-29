template <class I> auto slide (I f, I l, I p) -> std::pair<I,I> // Sean Parent - C++ Seasoning [GN2013]
{
    if (p < f) return { p, std::rotate (p, f, l) };
    if (l < p) return { std::rotate (f, l, p), p };
    return {f, l};
}

TEST_ON
{
    {
    }
};
