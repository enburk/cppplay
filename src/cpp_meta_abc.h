namespace test 
{
    template<class T, T v> struct integral_constant
    {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type;
        constexpr operator value_type ()   const noexcept { return value; }
        constexpr value_type operator ()() const noexcept { return value; } // since C++14
    };

    // Walter Brown - Modern Template Programming Part II [CppCon2014]

    using true_type  = std::integral_constant<bool, true>;
    using false_type = std::integral_constant<bool, false>;

    template <class T, class U> struct is_same       : false_type {};
    template <class T>          struct is_same<T, T> : true_type  {};

    template <class T,           class... P0toN> struct is_one_of;
    template <class T                          > struct is_one_of<T>               : false_type             {};
    template <class T,           class... P1toN> struct is_one_of<T, T,  P1toN...> : true_type              {};
    template <class T, class P0, class... P1toN> struct is_one_of<T, P0, P1toN...> : is_one_of<T, P1toN...> {};

    template <class T> using is_void = is_one_of <T, void, const void, volatile void, const volatile void>;

    template <class...> using void_t = void; // used for well-formed SFINAE

    template <class, class = void> struct has_type_member                              : false_type {};
    template <class T>             struct has_type_member<T, void_t<typename T::type>> : true_type  {};

    template <class T> using copy_assignment_t = decltype (std::declval<T&>() = std::declval<const T&>());

    template <class, class = void> struct is_copy_assignable : false_type {};
    template <class T>             struct is_copy_assignable<T, void_t<copy_assignment_t<T>>> : is_same<copy_assignment_t<T>, T&> {};
}

TEST_OFF
{
    TEST (test::is_one_of<std::string, std::vector<int>, std::string>::value);
    TEST (test::is_one_of<std::string, int, unsigned, float, double >::value);

    cout << endl;

    TEST (test::is_void<const void >::value);
    TEST (test::is_void<std::string>::value);

    cout << endl;

    TEST (test::has_type_member<test::true_type>::value);
    TEST (test::has_type_member<element        >::value);

    cout << endl;

    TEST (test::is_copy_assignable<std::shared_ptr<int>>::type::value);
    TEST (std ::is_copy_assignable<std::shared_ptr<int>>::type::value);
    TEST (test::is_copy_assignable<std::unique_ptr<int>>::type::value);
    TEST (std ::is_copy_assignable<std::unique_ptr<int>>::type::value);
};

// Output:
// 
// test::is_one_of<std::string, std::vector<int>, std::string>::value >>> 1
// test::is_one_of<std::string, int, unsigned, float, double >::value >>> 0
// 
// test::is_void<const void >::value >>> 1
// test::is_void<std::string>::value >>> 0
// 
// test::has_type_member<test::true_type>::value >>> 1
// test::has_type_member<element >::value >>> 0
// 
// test::is_copy_assignable<std::shared_ptr<int>>::type::value >>> 1
// std ::is_copy_assignable<std::shared_ptr<int>>::type::value >>> 1
// test::is_copy_assignable<std::unique_ptr<int>>::type::value >>> 0
// std ::is_copy_assignable<std::unique_ptr<int>>::type::value >>> 0
