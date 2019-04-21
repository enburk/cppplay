#define CHECK_TYPE(v,t) static_assert (std::is_same_v <decltype(v), t>)

// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c

template <class T> constexpr std::string_view type_name () // Howard Hinnant et al.
{
    using namespace std;

    #ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);

    #elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
    #if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
    #else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
    #endif

    #elif defined(_MSC_VER)
    string_view s = __FUNCSIG__;
    s.remove_prefix (84);
    s.remove_suffix (7);
    return s;
    #endif
}

TEST_OFF
{
    const int c = 0;
    const int & r = c;

    cout << type_name<decltype(c)>() << endl;
    cout << type_name<decltype(r)>() << endl;

    CHECK_TYPE (c, const int);
    CHECK_TYPE (r, const int &);

    cout << endl << type_name<decltype(detail::log::mutex)>() << endl;
    cout << endl << type_name<decltype(detail::log::print)>() << endl;
    cout << endl << type_name<decltype(detail::log::Log  )>() << endl;
};

// Output:
// 
// const int
// const int&
// 
// class std::recursive_mutex
// 
// void(void)
// 
// class std::vector<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,
// class std::allocator<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > > >
