#pragma once // https://en.wikipedia.org/wiki/Pragma_once#Portability

// You couldn't name your file 'aux.h' on Windows. As well as:
// CON, PRN, NUL, COM1, COM2, ... , COM9, LPT1, LPT2, ... , LPT9.

#include <vector>
#include <string>
#include <cassert>   
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;

struct FILENAME
{
    inline static std::string TEXT; FILENAME (const char * s) { TEXT = s; }
};

struct TESTCLASS
{
    template<class Test> TESTCLASS (Test test)
    {
        cout << endl << FILENAME::TEXT << endl << endl; test ();
    }
};

#define CONCAt(x,y)   x##y
#define CONCAT(x,y)   CONCAt (x,y)

// Macro __COUNTER__ is non-standard but widely supported
#define TEST_OFF auto      CONCAT (Test,__COUNTER__) = []()
#define TEST_ON  FILENAME  CONCAT (Test,__COUNTER__) = __FILE__; \
                 TESTCLASS CONCAT (Test,__COUNTER__) = []()

#define TESt(...) { cout << #__VA_ARGS__ << " >>> ";   __VA_ARGS__; }
#define TEST(...) { cout << #__VA_ARGS__ << " >>> " << __VA_ARGS__ << endl; }

TEST_OFF
{
    TEST (        "Test how test works."); cout << endl;
    TESt (cout << "Test how test works."); cout << endl;
};

#include "aux_log.h"
#include "aux_element.h"
#include "aux_chrono.h"
