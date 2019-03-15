#pragma once

#include <string>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

namespace detail
{
    struct Filename
    {
        inline static std::string text;
        Filename (const char * s) { text = s; }
    };

    struct TestClass
    {
        template<class Test> TestClass (Test test)
        {
            cout << endl << Filename::text << endl << endl;
            test ();
        }
    };
}

#define CONCAt(x,y) x##y
#define CONCAT(x,y) CONCAt (x,y)

#define TEST_OFF auto                     CONCAT (Test,__COUNTER__) = []()
#define TEST_ON  inline detail::Filename  CONCAT (Test,__COUNTER__) = __FILE__; \
                 inline detail::TestClass CONCAT (Test,__COUNTER__) = []()

#define TEST(...) { cout << #__VA_ARGS__ << " >>> " << __VA_ARGS__ << endl; }

TEST_ON
{
    TEST ("Test how test works.");
};

// Macro __COUNTER__ is non-standard but widely supported, though can cause ODR violation:
// https://stackoverflow.com/questions/37268686/how-can-counter-cause-a-odr-violation-here
