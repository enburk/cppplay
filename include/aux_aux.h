#pragma once // https://en.wikipedia.org/wiki/Pragma_once#Portability

// You couldn't name your file 'aux.h' on Windows. As well as:
// CON, PRN, NUL, COM1, COM2, ... , COM9, LPT1, LPT2, ... , LPT9.

// Macro __COUNTER__ is non-standard but widely supported, though can cause ODR violation:
// https://stackoverflow.com/questions/37268686/how-can-counter-cause-a-odr-violation-here

#include <vector>
#include <string>
#include <string_view>
#include <cassert>   
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::cout;
using std::endl;

namespace detail
{
    struct Filename
    {
        static std::string & text() noexcept {
        static std::string text_; return text_; }
        Filename (const char * s) { text() = s; }
    };

    struct TestClass
    {
        template<class Test> TestClass (Test test)
        {
            cout << endl << Filename::text() << endl << endl;
            test ();
        }
    };
}

#define CONCAt(x,y) x##y
#define CONCAT(x,y) CONCAt (x,y)

#define TEST_OFF inline auto              CONCAT (Test,__COUNTER__) = []()
#define TEST_ON  inline detail::Filename  CONCAT (Test,__COUNTER__) = __FILE__; \
                 inline detail::TestClass CONCAT (Test,__COUNTER__) = []()

#pragma warning(disable : 26426) // Global initializer calls a non-constexpr function

#define TESt(...) { cout << #__VA_ARGS__ << " >>> ";   __VA_ARGS__; }
#define TEST(...) { cout << #__VA_ARGS__ << " >>> " << __VA_ARGS__ << endl; }

TEST_OFF
{
    TEST (        "Test how test works."); cout << endl;
    TESt (cout << "Test how test works."); cout << endl;
};

// VS 2017 stable false positives:
#pragma warning(disable : 26486)
#pragma warning(disable : 26487)
#pragma warning(disable : 26489)

#include "aux_log.h"
#include "aux_element.h"
#include "aux_chrono.h"
#include "aux_test.h"

/*
STILL NOT SUPPORTED:

#include <experimental/source_location>

using loc = experimental::source_location;
 
void my_assert(bool test, const char* reason,
               loc location = loc::current())
{
    if (!test)
    {
         cout << "Assertion failed: " << location.file_name << ":" 
              << location.line << ":" << location.column << ": "
              << "in function " << location.function_name << ": "
              << reason << endl;
    }
}
 
TEST_ON
{   
    // will print out with correct filename,
    // line and column numbers, and function name.
    my_assert(false, "On purpose");
};
*/

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
