#pragma once

#include "aux_aux.h"
#include <optional>
#include <variant>
#include <clocale>
#if defined(_MSC_VER)
#include <intrin.h> // __readeflags
#endif

namespace tricky
{
    #include "cpp_tricky/tricky_conversion.h"
    #include "cpp_tricky/tricky_initialization.h"
    #include "cpp_tricky/tricky_initializer_list.h"
    #include "cpp_tricky/tricky_integer_overflow.h"
}
