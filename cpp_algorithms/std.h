SGI STL Documentation

// Sean Parent, "Generic Programming", Pacific++ talk, 2018
// STL implementation
template <
class I, // I models ForwardIterator
class T  // T is value_type(I)
>
auto lower_bound (I f, I l, const T& v) -> I
{
    while (f != l) {
        auto m = next(f, distance(f, l) / 2);
        if (*m < v) f = next(m);
        else l = m;
    }
    return f;
}

// https://en.cppreference.com/w/cpp/algorithm/lower_bound
template<class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
{
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);
 
    while (count > 0) {
        it = first; 
        step = count / 2; 
        std::advance(it, step);
        if (*it < value) {
            first = ++it; 
            count -= step + 1; 
        }
        else
            count = step;
    }
    return first;
}
