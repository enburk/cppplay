// Alisdair Meredith - Recreational C++ [CppCon 2017]
template <typename> constexpr bool is_function = false;
template <typename Result, typename... Args>
constexpr bool is_function<(Result(Args...))> = true;
template <typename Result, typename... Args>
constexpr bool is_function<(Result(Args...,...))> = true;
// ellipsis * cv-qualifier * ref-qualifier * noexcept = 2*4*3*2 = 48
// __fastcall ?
// Clang:
template <typename T>
constexpr bool is_function_v = !is_integral_v<T>
                           and !is_float_v<T>
                           and !is_enum_v<T>
                           and !is_array_v<T>
                           and !is_pointer_v<T>
                           and !is_member_pointer_v<T>
                           and !is_nullptr_v<T>
                           and !is_class_v<T>
                           and !is_union_v<T>
                           and !is_void_v<T>
                           and !is_lvalue_reference_v<T>
                           and !is_rvalue_reference_v<T>;
// Alisdair:
template <typename T>
constexpr bool is_function_v = !is_const_v<T>
                           and !is_reference_v<T>
                           and  is_same_v<T, const T>;
// Simplified by Johannes Schaub:
template <typename T>
constexpr bool is_function_v = !is_const_v<T const>
                           and !is_reference_v<T>;


template <typename T>
void operator+(T); // wouldn't be overloaded for fundamental types

template <typename T, typename=void>
constexpr bool is_integral_v = false;

template <typename T>
constexpr bool is_integral_v< T, void_t<
                            , decltype (+T{})& // filter out non-arithmetic types
                            , decltype (T{}%1) // filter out floats
                            >> = true;

