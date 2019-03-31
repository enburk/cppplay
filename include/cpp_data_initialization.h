TEST_ON
{
    oops ( element e        ;     ), { "ctor: _", "-------", "dtor: _" });
//  oops ( element e   ()   ;     ), {            "-------"            }); // it's a function!
    oops ( element e   {}   ;     ), { "ctor: _", "-------", "dtor: _" });
    oops ( element e = {}   ;     ), { "ctor: _", "-------", "dtor: _" });
    oops ( element e   ('a');     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e   {'a'};     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e = ('a');     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e = {'a'};     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e =  'a' ;     ), { "ctor: a", "-------", "dtor: a" });
    oops ( auto e = element();    ), { "ctor: _", "-------", "dtor: _" });
    oops ( auto e = element{};    ), { "ctor: _", "-------", "dtor: _" });
    oops ( auto e = element('a'); ), { "ctor: a", "-------", "dtor: a" });
    oops ( auto e = element{'a'}; ), { "ctor: a", "-------", "dtor: a" });

// CppCon 2018: Nicolai Josuttis “The Nightmare of Initialization in C++”

    #define CHECK_TYPE(v,t) static_assert (std::is_same_v <decltype(v), t>)
    {
              int  i01; /* undefined value */ i01 = 42; // prevent warning
        const int  i02 = 42;   
        const int  i03 (42);    
        const int  i04 = int();
    
        const int  i05 {42};    
        const int  i06 {};      
        const int  i07 = {42};   
        const int  i08 = {};   
    
        const auto i09  = 42;  // int
        const auto i10   {42}; // int (C++14), std::initializer_list<int> (C++11)
        const auto i11 = {42}; CHECK_TYPE (i11, const std::initializer_list<int>);
        const auto i12 = int{42}; // int
    
        const int  i13 (); // declares a function

    //  const int  i14   (7,9);    // compile-time error
        const int  i15 = (7,9);    // OK, 9 (comma operator)
    //  const int  i16 = int(7,9); // compile-time error
    //  const auto i17   (7,9);    // compile-time error
        const auto i18 = (7,9);    // OK, int 9 (comma operator);
    //  const auto i19 = int(7,9); // compile-time error;

        enum class Salutation {mr, mrs};
    //  const Salutation s1 =  0;  // error
    //  const Salutation s2   (0); // error
        const Salutation s3   {0}; // OK since C++17
    //  const Salutation s4 = {0}; // error

        enum  MyInt : unsigned long long {};
    //  const MyInt i1 =  0;  // error
    //  const MyInt i2   (0); // error
        const MyInt i3   {0}; // OK since C++17
    //  const MyInt i4 = {0}; // error

        enum  Flag {bit1 = 1, bit2 = 2, bit3 = 4};
    //  const Flag f {1}; // error

        const std::byte b {0b1111'1111}; // OK since C++17
    }
    {
        using namespace std::literals;
        using namespace std::chrono_literals;
        const auto s1 = "abc";  CHECK_TYPE (s1, const char* const);
        const auto s2 = "abc"s; CHECK_TYPE (s2, const std::string); // OK since C++14
        const auto s3 = 12345s; CHECK_TYPE (s3, const std::chrono::seconds);

        const auto v = std::array<int,1000>{{0}}; // fast since C++17

        const auto a = std::atomic<int>{9}; // OK since C++17

        const auto e = neither_copyable_nor_movable ('e'); // OK since C++17, inplace constructing

        const auto & r = neither_copyable_nor_movable ('r'); // temporary return value "materializes"

        //            expression
        //       glvalue      rvalue
        // lvalue       xvalue       prvalue
        //                     <----
        //                     temporary
        //                     materialization
        //                     conversion

        // // auto decay:
        // const int aa [9] = {0}; auto r1 = aa; CHECK_TYPE (r1, const int *); // raw arrays convert to pointers
        // const int & r2 = aa[0]; auto r3 = r2; CHECK_TYPE (r3, int); // not a reference but a new object

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

    #undef CHECK_TYPE
};

struct TestCtor
{
    TestCtor (int n = 0)                  noexcept(false) { log::put ("TestCtor (int n = 0)"); }
    TestCtor (int n1, int n2)             noexcept(false) { log::put ("TestCtor (int n1, int n2)"); }
    TestCtor (std::initializer_list<int>) noexcept(false) { log::put ("TestCtor (std::initializer_list<int>)"); }
};

TEST_ON
{
    oops ( const TestCtor t;         ), {"TestCtor (int n = 0)", "-------"});
    oops ( const TestCtor t = 42;    ), {"TestCtor (int n = 0)", "-------"});

//  oops ( const TestCtor t   ();    ), {"TestCtor (int n = 0)", "-------"}); // it's a function
    oops ( const TestCtor t   (1);   ), {"TestCtor (int n = 0)", "-------"});
    oops ( const TestCtor t   (1,2); ), {"TestCtor (int n1, int n2)", "-------"});

    oops ( const TestCtor t   {};    ), {"TestCtor (int n = 0)", "-------"});
    oops ( const TestCtor t   {1};   ), {"TestCtor (std::initializer_list<int>)", "-------"});
    oops ( const TestCtor t   {1,2}; ), {"TestCtor (std::initializer_list<int>)", "-------"});

    oops ( const TestCtor t = {};    ), {"TestCtor (int n = 0)", "-------"});
    oops ( const TestCtor t = {1};   ), {"TestCtor (std::initializer_list<int>)", "-------"});
    oops ( const TestCtor t = {1,2}; ), {"TestCtor (std::initializer_list<int>)", "-------"});

    TESt ( std::vector<int> v (3, 42); cout << v << endl; ); // Output: 42 42 42
    TESt ( std::vector<int> v {3, 42}; cout << v << endl; ); // Output: 3 42

    TESt ( std::vector<std::string> v = {}; cout << v << endl; ); //
    TESt ( std::vector<std::string> v = {"1"}; cout << v << endl; ); // 1
    TESt ( std::vector<std::string> v = {"1","2"}; cout << v << endl; ); // 1 2

    TESt ( std::vector<std::string> v = {{{"1"},{"2"}}}; cout << v << endl; ); // 1 2
    TESt ( std::vector<std::string> v = {{"1","2","3"}}; cout << v << endl; ); // 1 2 3
//  TESt ( std::vector<std::string> v = {{{"1","2"}}};   cout << v << endl; ); // run-time error (pair of iterators)
//  TESt ( std::vector<std::string> v = {{"1","2"}};     cout << v << endl; ); // run-time error (pair of iterators)
};


/*
struct A {
    int i;
};

int main() {
    A a = {.i = 0}; // C designated initializers aren’t supported in C++ until C++20
    std::cout << a.i << std::endl;
}*/
