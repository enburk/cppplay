struct TestCtor
{
    TestCtor (int n = 0)  noexcept(false) { log::put ("TestCtor (int n = 0)"); }
    TestCtor (int n1, int n2)             { log::put ("TestCtor (int n1, int n2)"); }
    TestCtor (std::initializer_list<int>) { log::put ("TestCtor (std::initializer_list<int>)"); }
};

TEST_OFF
{
    // CppCon 2018: Nicolai Josuttis “The Nightmare of Initialization in C++”
    const auto i10   {42}; // int (C++14), std::initializer_list<int> (C++11)
    const auto i11 = {42}; CHECK_TYPE (i11, const std::initializer_list<int>);

    oops ( TestCtor t;         ) {"TestCtor (int n = 0)", "-------"};
    oops ( TestCtor t = 42;    ) {"TestCtor (int n = 0)", "-------"};

//  oops ( TestCtor t   ();    ) {"TestCtor (int n = 0)", "-------"}; // it's a function
    oops ( TestCtor t   (1);   ) {"TestCtor (int n = 0)", "-------"};
    oops ( TestCtor t   (1,2); ) {"TestCtor (int n1, int n2)", "-------"};

    oops ( TestCtor t   {};    ) {"TestCtor (int n = 0)", "-------"};
    oops ( TestCtor t   {1};   ) {"TestCtor (std::initializer_list<int>)", "-------"};
    oops ( TestCtor t   {1,2}; ) {"TestCtor (std::initializer_list<int>)", "-------"};

    oops ( TestCtor t = {};    ) {"TestCtor (int n = 0)", "-------"};
    oops ( TestCtor t = {1};   ) {"TestCtor (std::initializer_list<int>)", "-------"};
    oops ( TestCtor t = {1,2}; ) {"TestCtor (std::initializer_list<int>)", "-------"};

    using std::vector;
    using std::string;

    TESt ( vector<int> v (3, 42); cout << v << endl; ); // Output: 42 42 42
    TESt ( vector<int> v {3, 42}; cout << v << endl; ); // Output: 3 42

    TESt ( vector<string> v = {};        cout << v << endl; ); //
    TESt ( vector<string> v = {"1"};     cout << v << endl; ); // 1
    TESt ( vector<string> v = {"1","2"}; cout << v << endl; ); // 1 2

    TESt ( vector<string> v = {{{"1"},{"2"}}}; cout << v << endl; ); // 1 2
    TESt ( vector<string> v = {{"1","2","3"}}; cout << v << endl; ); // 1 2 3
//  TESt ( vector<string> v = {{{"1","2"}}};   cout << v << endl; ); // run-time error (pair of iterators)
//  TESt ( vector<string> v = {{"1","2"}};     cout << v << endl; ); // run-time error (pair of iterators)
};

inline static std::vector<size_t> mallocs;
/* fatal error C1001: An internal error has occurred in the compiler.

template <class type> struct mallocounter
{
    type * allocate(size_t n) { mallocs.push_back (n); return new type [n]; }
    void deallocate(type * p, size_t) noexcept { delete [] p; }
};
*/

template <class T> struct mallocounter
{
    using value_type = T;

    template <class U> constexpr
    mallocounter(const mallocounter<U>&) noexcept {}
    mallocounter() = default;

    #pragma warning (push)
    #pragma warning (disable: 26408) // Avoid malloc() and free(), prefer new with delete
    
    [[nodiscard]] T* allocate(size_t n) {
        mallocs.push_back (n);
        if(n > size_t(-1) / sizeof(T)) throw std::bad_alloc();
        if(auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) return p;
        throw std::bad_alloc();
    }
    void deallocate(T* p, size_t) noexcept { std::free(p); }
    #pragma warning (pop)
};
template <class T, class U> bool operator==(const mallocounter<T>&, const mallocounter<U>&) { return true; }
template <class T, class U> bool operator!=(const mallocounter<T>&, const mallocounter<U>&) { return false; }

TEST_OFF // CppCon 2018: Jason Turner “Surprises in Object Lifetime”
{
    // initializer_list vs. initializer list of aggregate

    using String = std::basic_string<char, std::char_traits<char>, mallocounter<char>>;
    using Vector = std::vector<String, mallocounter<String>>;
    using Array = std::array <String, 2>;
    using array = std::array<std::string, 2>;

    #define s1 "string long enough to prevent small string optimization"
    #define s2 "123456789_123456789_123456789_123456789_123456789_123456789_"

    Vector v1 {s1, s2}; cout << mallocs << endl; mallocs.clear(); // Output: 64 64 2 64 64
    Array  a1 {s1, s2}; cout << mallocs << endl; mallocs.clear(); // Output: 64 64
    array  a2 {s1, s2}; // zero allocations, array of const char *

    #undef s1
    #undef s2
};
