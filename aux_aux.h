#pragma once // https://en.wikipedia.org/wiki/Pragma_once#Portability

// You couldn't name your file 'aux.h' on Windows. As well as:
// CON, PRN, NUL, COM1, COM2, ... , COM9, LPT1, LPT2, ... , LPT9.

// Macro __COUNTER__ is non-standard but widely supported, though can cause ODR violation:
// https://stackoverflow.com/questions/37268686/how-can-counter-cause-a-odr-violation-here

#include <array>
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

struct TestTitle { TestTitle (const char * s) { cout << endl << s << endl << endl; } };

struct TestClass { template<class Test> TestClass (Test test) { test (); } };

#define CONCAt(x,y) x##y
#define CONCAT(x,y) CONCAt (x,y)

#define TEST_OFF inline auto      CONCAT (Test,__COUNTER__) = []() noexcept(false) 
#define TEST_ON  inline TestClass CONCAT (Test,__COUNTER__) = [](TestTitle =__FILE__) noexcept(false) 

#pragma warning(disable : 26426) // Global initializer calls a non-constexpr function

#define TESt(...) { cout << #__VA_ARGS__ << " >>> ";   __VA_ARGS__; }
#define TEST(...) { cout << #__VA_ARGS__ << " >>> " << __VA_ARGS__ << endl; }

TEST_OFF
{
    TEST (        "Test how test works."); cout << endl;
    TESt (cout << "Test how test works."); cout << endl;
};

// Disable some warnings to keep tests less verbose
#pragma warning(disable : 26496) // The variable is assigned only once, mark it as const
#pragma warning(disable : 26439) // This kind of function may not throw. Declare it 'noexcept'
#pragma warning(disable : 26455) // Default constructor may not throw. Declare it 'noexcept'

// VS 2017 stable false positives:
#pragma warning(disable : 26486)
#pragma warning(disable : 26487)
#pragma warning(disable : 26489)

#include "aux_log.h"
#include "aux_element.h"
#include "aux_chrono.h"
#include "aux_test.h"
#include "aux_type.h"

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
