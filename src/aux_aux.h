#pragma once // https://en.wikipedia.org/wiki/Pragma_once#Portability

// You couldn't name your file 'aux.h' on Windows. As well as:
// CON, PRN, NUL, COM1, COM2, ... , COM9, LPT1, LPT2, ... , LPT9.

// Macro __COUNTER__ is non-standard but widely supported.

#include <string>
#include <iomanip>
#include <iostream>
#define cout std::cout
#define endl std::endl

struct FILENAME
{
    inline static std::string TEXT; FILENAME (const char * s) { TEXT = s; }
};

struct TESTCLASS
{
    template<class Lambda> TESTCLASS (Lambda test)
    {
        cout << endl << FILENAME::TEXT << endl << endl; test ();
    }
};

#define CONCAt(x,y)   x##y
#define CONCAT(x,y)   CONCAt (x,y)

#define TEST FILENAME CONCAT (Test,__COUNTER__) = __FILE__; TESTCLASS

#define TEST_ON  TEST CONCAT (Test,__COUNTER__) = []()

#define TEST_OFF auto CONCAT (Test,__COUNTER__) = []()

TEST_ON
{
    cout << "Test how test works." << endl;
};


#include "aux_log.h"
#include "aux_chrono.h"
