// R. Martinho Fernandes, “Some pitfalls with forwarding constructors”.
// Eric Niebler, “Universal References and the Copy Constructor”.
// Scott Meyers, “Copying Constructors in C++11 “
struct T0
{ 
    T0 (          ) { cout << "T0 ctor" << endl; }
    T0 (const T0 &) { cout << "T0 copy ctor" << endl; }
};
struct T1
{
    T0 t0;
    T1 () = default;
    template<typename TT>
    T1 (const TT &) { cout << "T1 template ctor" << endl; }
    // true copy constructor would be automatically generated
};
struct T2
{
    T0 t0;
    T2 () = default;
    template<typename TT>
    T2 (const TT &) { cout << "T2 template ctor" << endl; }
    T2 (const T2 &) { cout << "T2 copy ctor" << endl; }
};
struct T3
{
    T0 t0;
    T3 () = default;
    template<typename TT>
    T3 (const TT &) { cout << "T3 template ctor" << endl; }
    T3 (const T3 &) { cout << "T3 copy ctor" << endl; }
    T3 (T3 &&) noexcept
                    { cout << "T3 universal copy ctor" << endl; }
};
struct T4
{
    T0 t0;
    T4 () = default;
    template<typename TT>
    T4 (const TT &) { cout << "T4 template ctor" << endl; }
    T4 (const T4 &) { cout << "T4 copy ctor" << endl; }
    T4 (T4 &&) noexcept
                    { cout << "T4 universal copy ctor" << endl; }
    template<typename TT> // better match for lvalue T4 than the copy constructor
    T4 (TT &&) { cout << "T4 template universal copy ctor" << endl; }
};

TEST_OFF
{
    { T1 u; T1 v = u; }; cout << endl;
    // T0 ctor
    // T0 copy ctor
    { T2 u; T2 v = u; }; cout << endl;
    // T0 ctor
    // T0 ctor
    // T2 copy ctor
    { T3 u; T3 v = u; }; cout << endl;
    // T0 ctor
    // T0 ctor
    // T3 copy ctor
    { T4 u; T4 v = u; }; cout << endl;
    // T0 ctor
    // T0 ctor
    // T4 template universal copy ctor
    { const T4 u; T4 v = u; }; // template universal copy ctor lacks const-qualifier
    // T0 ctor
    // T0 ctor
    // T4 copy ctor
};

