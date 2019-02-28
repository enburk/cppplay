#pragma once
#include "aux_aux.h"

namespace trap
{
    #include "cpp_trap_decltype.h"
    #include "cpp_trap_forward.h"
    #include "cpp_trap_references.h"
    #include "cpp_trap_shadowing.h"

    // https://www.reddit.com/r/programming/comments/1m1izv/goingnative_2013_writing_quick_code_in_c_quickly/
    // andralex: Consider noexcept. This function compiles flag free!
    // void fun() noexcept { throw "so sue me"; }
    // VS 2017: warning C4297: 'trap::fun': function assumed not to throw an exception but does

}
