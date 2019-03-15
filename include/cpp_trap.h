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

    TEST_ON // Victor Zverovich - A modern formatting library for C++ [CppCon 2017]
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

    TEST_ON // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]
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
    string fn (string s = "foo", bool b = true) { return (b) ? s : ""; }

    TEST_ON
    {
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
        virtual string fn (string s = "foo")
        {
            return s;
        }
    };

    struct Derived : Base
    {
        virtual string fn (string s = "bar") override
        {
            return s + "!!!";
        }
    };

    TEST_ON
    {
        Base b;
        assert(b.fn() == "foo");

        Derived d;
        assert(d.fn() == "bar!!!");

        Base & b_ref_to_d = d;
        assert(b_ref_to_d.fn() == "foo!!!");
    };


}
