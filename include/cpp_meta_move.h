namespace test
{
    template<class T> struct remove_reference      { using type = T; }; // remove reference
    template<class T> struct remove_reference<T&>  { using type = T; }; // remove reference
    template<class T> struct remove_reference<T&&> { using type = T; }; // remove rvalue reference
    template<class T> using  remove_reference_t = typename remove_reference<T>::type;

    template<class T> struct is_lvalue_reference     : false_type {};
    template<class T> struct is_lvalue_reference<T&> : true_type  {};

    template<class T> constexpr T&& forward (remove_reference_t<T>& arg) noexcept
    {
        return static_cast<T&&>(arg); // forward an lvalue as either an lvalue or an rvalue
    }
    template<class T> constexpr T&& forward (remove_reference_t<T>&& arg) noexcept
    {
        static_assert (!is_lvalue_reference<T>::value, "bad forward call");

        return static_cast<T&&>(arg); // forward an rvalue as an rvalue
    }
    template<class T> constexpr remove_reference_t<T>&& move (T&& arg) noexcept
    {
        return static_cast<remove_reference_t<T>&&>(arg); // forward arg as movable
    }
}

TEST_ON
{
    {
        auto f1 = [](element &  e) { return test::forward<element> (e); };
        auto f2 = [](element && e) { return test::forward<element> (e); };

        element a {'a'};
        element b {'b'};

        log::print ();

        auto aa = f1 (a);
    //  auto bb = f2 (b); // compile error
        auto bb = f2 (std::move (b));

        log::print ();

    //  auto cc = f1 (element {'c'}); // compile error
        auto cc = f2 (element {'c'});
        auto dd = f2 (element {'d'});

        log::print ();

        cc = test::forward<element> (aa);
        dd = test::move             (bb);

        log::print ();
    }

    log::print ();
};
