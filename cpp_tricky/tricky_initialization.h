TEST_ON
{
    oops ( element e        ;     ) { "ctor: _", "-------", "dtor: _" };
//  oops ( element e   ()   ;     ) {            "-------"            }; // it's a function!
    oops ( element e   {}   ;     ) { "ctor: _", "-------", "dtor: _" };
    oops ( element e = {}   ;     ) { "ctor: _", "-------", "dtor: _" };
    oops ( element e   ('a');     ) { "ctor: a", "-------", "dtor: a" };
    oops ( element e   {'a'};     ) { "ctor: a", "-------", "dtor: a" };
    oops ( element e = ('a');     ) { "ctor: a", "-------", "dtor: a" };
    oops ( element e = {'a'};     ) { "ctor: a", "-------", "dtor: a" };
    oops ( element e =  'a' ;     ) { "ctor: a", "-------", "dtor: a" };
    oops ( auto e = element();    ) { "ctor: _", "-------", "dtor: _" };
    oops ( auto e = element{};    ) { "ctor: _", "-------", "dtor: _" };
    oops ( auto e = element('a'); ) { "ctor: a", "-------", "dtor: a" };
    oops ( auto e = element{'a'}; ) { "ctor: a", "-------", "dtor: a" };

    // CppCon 2018: Nicolai Josuttis “The Nightmare of Initialization in C++”
    {
        [[maybe_unused]]
        int  i01; /* undefined value */ i01 = 42; // prevent warning
        int  i02 = 42;   
        int  i03 (42);    
        int  i04 = int();
    
        int  i05 {42};    
        int  i06 {};      
        int  i07 = {42};   
        int  i08 = {};   
    
        auto i09  = 42;  // int
        auto i10   {42}; // int (C++14), std::initializer_list<int> (C++11)
        auto i11 = {42}; ///CHECK_TYPE (i11, std::initializer_list<int>);
    //cout << endl << type_name<decltype(i11)>() << endl;
        auto i12 = int{42}; // int
    
        int  i13 (); // declares a function

    //  int  i14   (7,9);    // compile-time error
        int  i15 = (7,9);    // OK, 9 (comma operator)
    //  int  i16 = int(7,9); // compile-time error
    //  auto i17   (7,9);    // compile-time error
        auto i18 = (7,9);    // OK, int 9 (comma operator);
    //  auto i19 = int(7,9); // compile-time error;

        enum class Salutation {mr, mrs};
    //  Salutation s1 =  0;  // error
    //  Salutation s2   (0); // error
        Salutation s3   {0}; // OK since C++17
    //  Salutation s4 = {0}; // error

        enum MyInt : unsigned long long {};
    //  MyInt i1 =  0;  // error
    //  MyInt i2   (0); // error
        MyInt i3   {0}; // OK since C++17
    //  MyInt i4 = {0}; // error

        enum Flag {bit1 = 1, bit2 = 2, bit3 = 4};
    //  Flag f {1}; // error

        const std::byte b {0b1111'1111}; // OK since C++17
    }
    {
        using namespace std::literals;
        using namespace std::chrono_literals;
        auto s1 = "abc";  CHECK_TYPE (s1, const char*);
        auto s2 = "abc"s; CHECK_TYPE (s2, std::string); // OK since C++14
        auto s3 = 12345s; CHECK_TYPE (s3, std::chrono::seconds);

///     auto v = std::array<int,1000>{{0}}; // fast since C++17

        auto a = std::atomic<int>{9}; // OK since C++17

        // But:
        // std::atomic<int> a1;   // formally not allowed to zero initialize (will be fixed in C++20)
        // std::atomic<int> a2{}; // formally not allowed to zero initialize (will be fixed in C++20)

        auto e = neither_copyable_nor_movable ('e'); // OK since C++17, inplace constructing

        const auto & r = neither_copyable_nor_movable ('r'); // temporary return value "materializes"

        //            expression
        //       glvalue      rvalue
        // lvalue       xvalue       prvalue
        //                     <----
        //                     temporary
        //                     materialization
        //                     conversion

        // // auto decay:
        // int aa [9] = {0}; auto r1 = aa; CHECK_TYPE (r1, const int *); // raw arrays convert to pointers
        // int & r2 = aa[0]; auto r3 = r2; CHECK_TYPE (r3, int); // not a reference but a new object

        log::print ();
    }
    log::print ();

    // Output:
    // 
    // ctor: e
    // move: e
    // dtor:
    // ctor: r
    // move: r
    // dtor:
    // =====
    // dtor: r
    // dtor: e
    // =====
};

// https://mikelui.io/2019/01/03/seriously-bonkers.html
// Initialization in C++ is Seriously Bonkers by Mike Lui

struct an_aggregate_1 { int i; };
struct an_aggregate_2 { int i; an_aggregate_2 () = default; };
//struct not_an_aggregate_1 { int i; not_an_aggregate_1 (){} }; // warning C26495: Variable is uninitialized
//struct not_an_aggregate_2 { int i; not_an_aggregate_2 (); };  // warning C26495: Variable is uninitialized
//not_an_aggregate_2::not_an_aggregate_2 () = default;

TEST_OFF
{
        an_aggregate_1 a1{}; cout << a1.i << endl;
        an_aggregate_2 a2{}; cout << a2.i << endl;
//  not_an_aggregate_1 b1{}; cout << b1.i << endl; // warning C26495: Variable is uninitialized
//  not_an_aggregate_2 b2{}; cout << b2.i << endl; // warning C26495: Variable is uninitialized
//  not_an_aggregate_2 c = {.i = 0}; // C designated initializers aren’t supported in C++ until C++20
    
};

