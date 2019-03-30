#include <memory>
std::uninitialized_fill _n std::par_unseq
std::uninitialized_copy _n std::par_unseq
std::uninitialized_move _n std::par_unseq
std::uninitialized_default_construct _n std::par_unseq
std::uninitialized_value_construct _n std::par_unseq
std::destroy

    void ctor (integer index, const type &  copy) { new (data + index) type (copy); }
    void ctor (integer index,       type && movy) { new (data + index) type (std::move (movy)); }
    void dtor (integer index)                     { data [index].~type (); }
    void dtor ()                                  { for (type* p = data; p < data+size; p++) p->~type (); }

template<class ForwardIt, class T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
{
    typedef typename std::iterator_traits<ForwardIt>::value_type Value;
    ForwardIt current = first;
    try {
        for (; current != last; ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) Value(value);
        }
    }  catch (...) {
        for (; first != current; ++first) {
            first->~Value();
        }
        throw;
    }
}

template<class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last)
{
    using Value = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
        for (; current != last; ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) Value;
        }
    }  catch (...) {
        std::destroy(first, current);
        throw;
    }
}

template<class ForwardIt>
void uninitialized_value_construct(ForwardIt first, ForwardIt last)
{
    using Value = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt current = first;
    try {
        for (; current != last; ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) Value();
        }
    } catch (...) {
        std::destroy(first, current);
        throw;
    }
}