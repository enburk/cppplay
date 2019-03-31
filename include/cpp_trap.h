#pragma once
#include "aux_aux.h"
#include <clocale>
#include <optional>
#include <variant>

namespace trap
{
    #include "cpp_trap_decltype.h"
    #include "cpp_trap_forward.h"
    #include "cpp_trap_references.h"
    #include "cpp_trap_shadowing.h"
    #include "cpp_trap_sql.h"

    // https://www.reddit.com/r/programming/comments/1m1izv/goingnative_2013_writing_quick_code_in_c_quickly/
    // andralex: Consider noexcept. This function compiles flag free!
    // void fun() noexcept { throw "so sue me"; }
    // VS 2017: warning C4297: 'trap::fun': function assumed not to throw an exception but does

    TEST_OFF // Victor Zverovich - A modern formatting library for C++ [CppCon 2017]
    {
        // std::ofstream ofs ("test.json");
        // ofs << "{'value':" << 4.2 << "}";
        std::setlocale (LC_NUMERIC, "de_DE.UTF-8");
        cout   << "{'value':" << 4.2 << "}" << endl; // 4,2
        std::setlocale (LC_ALL, "C");
        cout   << "{'value':" << 4.2 << "}" << endl;
    };

    class Car { // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]
    public:
        static constexpr int MAX_SPEED = 300; // in km/h
        // Returns the current speed in km/h.
        std::optional<int> get_speed() const;
        // Returns nullopt if the speedometer is non-functional.
        bool can_accelerate() const {
            return get_speed() < MAX_SPEED; }
        // Not a compile-time error!
        // bool operator<(const optional<T> &, const U &); is used, and
        // nullopt is considered less than any T!
    };

    TEST_OFF // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]
    {
        struct nasty { operator int() { throw 42; } };
        std::variant<int, float> v = 1.1f;
        v.emplace<int>(nasty{});
        // v.valueless_by_exception() == true
    };

    template <typename T>
    struct id { using type = T; };

    TEST_OFF // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]
    {
        using std::string;

        std::variant<string, bool> v("abc");

        struct FUN {
            //id<string> operator()(string arg) const { cout << arg << endl; }
            //id<bool>   operator()(bool   arg) const { cout << arg << endl; }
            void operator()(string arg) const { cout << "string: " << arg << endl; }
            void operator()(bool   arg) const { cout << "bool: "   << arg << endl; }
        };
        //typename invoke_result_t<FUN, decltype("abc")>::type

        std::visit (FUN{}, v); // Output: bool: 1 // (boolshit)
    };



    // https://stackoverflow.com/questions/16588750/cs-most-vexing-parse-again
    // assume 'x' is a variable defined somewhere:
    // widget w(gadget(x));
    // How could that be interpreted as a function declaration? I mean, x is a variable, right?
    // Well, when declaring a variable you can add extra parenthesis, so gadget x; and gadget (x); both declare the same variable x.
    // The same applies to function arguments so the code above looks like a declaration of a function that takes a first argument named x of type gadget and returns a widget...



    // Function Default Arguments - Slingshot or Shotgun by Michael Price [CppCon 2017]
    std::string fn (std::string s = "foo", bool b = true) { return (b) ? s : ""; }

    TEST_OFF
    {
        using std::string;

        assert(fn() == "foo");
        {
            string fn (string, bool); // Hides previous DFAs!!!
        //  assert(fn()      == "foo"); // No longer compiles!
        //  assert(fn("bar") == "bar"); // No longer compiles!
        }
        {
            string fn (string, bool = false);  // Hides previous DFAs!!!
            string fn (string = "baz", bool); // bool = false (because of previous declaration)
            // Both parameters now have DFAs.
            assert(fn() == "");
        }
    };

    struct Base
    {
        virtual std::string fn (std::string s = "foo")
        {
            return s;
        }
    };

    struct Derived : Base
    {
        virtual std::string fn (std::string s = "bar") override
        {
            return s + "!!!";
        }
    };

    TEST_OFF
    {
        Base b;
        assert(b.fn() == "foo");

        Derived d;
        assert(d.fn() == "bar!!!");

        Base & b_ref_to_d = d;
        assert(b_ref_to_d.fn() == "foo!!!");
    };

    // Undefined Behavior in 2017 by John Regehr [CppCon 2017]
    // Undefined behavior optimization:
    void bar(char *p) {}
    void foo(char *p) {
    #ifdef DEBUG
        printf("%s\n", p);
    #endif
        if (p)
            bar(p);
    }
    // Without -DDEBUG
    //  foo:
    //      testq %rdi, %rdi
    //      je L1
    //      jmp _bar
    //  L1: ret
    //
    // With -DDEBUG
    //  foo:
    //      pushq %rbx
    //      movq %rdi, %rbx
    //      call _puts      ; UB if p == nullptr
    //      movq %rbx, %rdi
    //      popq %rbx
    //      jmp _bar        ; unconditional!

    int foo (int x) {
    return (x + 1) > x; // TRUE or UB
    }
    TEST_ON
    {
        // cout << ((INT_MAX + 1) > INT_MAX) << " "; // VS 2017: warning C4307: '+': integral constant overflow
        cout << foo(INT_MAX) << "\n"; // VS 2017: no warning!
        // Possible output:
        // 0 1 (GCC, Clang)
        // 0 0 (VS 2017)

        // int *p = (int*)malloc(sizeof(int));
        // int *q = (int*)realloc(p, sizeof(int));
        // *p = 1; // VS 2017: warning C6011: Dereferencing NULL pointer 'p'.
        // *q = 2;
        // if (p == q)
        //     printf("%d %d\n", *p, *q);
        // // Possible output:
        // // 1 2 (Clang)
        // // (VS 2017)

    };



// Undefined Behavior in 2017 by John Regehr [CppCon 2017]
int fermat() {
    const int MAX = 1000;
    int a = 1, b = 1, c = 1;
    while (1) {
        if (((a * a * a) == ((b * b * b) + (c * c * c))))
            return 1;
        a++;
        if (a > MAX) {
            a = 1;
            b++;
        }
        if (b > MAX) {
            b = 1;
            c++;
        }
        if (c > MAX) {
            c = 1;
        }
    }
    return 0;
}

TEST_OFF
{
    if (fermat())
        cout << "Fermat's Last Theorem disproved!\n";
    else
        cout << "Nope.\n";

    //  $ clang++ -O fermat.cpp
    //  $ ./a.out
    //  Fermat's Last Theorem disproved!
    //  $
};

TEST_OFF
{
    using namespace std::literals;
    TEST (std::string     ("abc\0\0def"  ).size());
    TEST (std::string     ("abc\0\0def"s ).size());
    TEST (std::string_view("abc\0\0def"  ).size());
    TEST (std::string_view("abc\0\0def"sv).size());
};

TEST_OFF
{
    auto f = [](const std::vector<int> & src)
    {
        std::vector<int> dst;
        std::copy (src.begin(), src.end(), dst.begin());
    };
};

}
