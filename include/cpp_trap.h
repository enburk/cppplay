#pragma once
#include "aux_aux.h"

namespace trap
{

    // https://stackoverflow.com/questions/4762662/are-parentheses-around-the-result-significant-in-a-return-statement

    // If the argument is either the unparenthesised name of an object/function, 
    // or is a member access expression (object.member or pointer->member), 
    // then the decltype specifies the declared type of the entity specified by this expression.
    // 
    // If the argument is any other expression of type T, then
    // 
    // a) if the value category of expression is xvalue, then the decltype specifies T&&
    // 
    // b) if the value category of expression is lvalue, then the decltype specifies T&
    // 
    // c) otherwise, decltype specifies T

    struct A
    {
        element e;

        decltype(auto) func1 () { return e;  } // return type is element
        decltype(auto) func2 () { return(e); } // return type is element&

        decltype(auto) func3 () { e = 'a'; return e;  } // return type is element
        decltype(auto) func4 () { e = 'b'; return(e); } // return type is element&
    };


    TEST_OFF
    {
    //  element & e1 = A ().func1 (); // compile error
        element & e2 = A ().func2 (); // compiles successfully

        A a1 {'a'}; auto b1 = a1.func1 (); b1 = 'b'; cout << a1.e.s << endl;
        A a2 {'a'}; auto b2 = a2.func2 (); b2 = 'b'; cout << a2.e.s << endl; // b2 : element

        log::print ();

        element ee1 = a1.func3 ();
        element ee2 = a1.func4 ();

        log::print ();

    };


    // Louis Brandy - Curiously Recurring C++ Bugs at Facebook [CppCon2017]

    class sync_object
    {
        std::mutex m_mutex;

        void do_the_mutation () { /* ... */ }

        public: void update () noexcept
        {
            std::unique_lock<std::mutex> (m_mutex); // is a declaration of shadowing local variable m_mutex, same as:
        //  std::unique_lock<std::mutex>  m_mutex ; // error C2086: 'std::unique_lock<std::mutex> m_mutex': redefinition

        //  std::lock_guard <std::mutex> (n_mutex); // error C2512: 'std::lock_guard<std::mutex>': no appropriate default constructor available
        //  std::scoped_lock<std::mutex> (n_mutex); // error C2512: 'std::scoped_lock<std::mutex>': no appropriate default constructor available

            do_the_mutation ();
        }

        void this_stuff_will_be_successfully_compile_also ()
        {
            std::string (foo);
        //  std::string {foo};  // VC 2017: warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
        //  std::string("foo"); // VC 2017: warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
        }
    };


    // https://www.reddit.com/r/programming/comments/1m1izv/goingnative_2013_writing_quick_code_in_c_quickly/
    // andralex: Consider noexcept. This function compiles flag free!
    // void fun() noexcept { throw "so sue me"; } // VS 2017: warning C4297: 'trap::fun': function assumed not to throw an exception but does

}
