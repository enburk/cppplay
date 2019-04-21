// What is Callable?

// CppCon 2018: Vincent Reverdy, From Metaprogramming Tricks to Elegance 

// Functions (48 types)                       // Function lvalue-references (48 types)         // Function rvalue-references (48 types)          // Function pointers (48 types)

// Basic versions (4 types )                  // Basic versions (4 types )                     // Basic versions (4 types )                      // Basic versions (4 types )
R(Args ...);                                  R(&)(Args ...);                                  R(&&)(Args ...);                                  R(*)(Args ...);
R(Args ... , ...);                            R(&)(Args ... , ...);                            R(&&)(Args ... , ...);                            R(*)(Args ... , ...);
R(Args ...) noexcept ;                        R(&)(Args ...) noexcept ;                        R(&&)(Args ...) noexcept ;                        R(*)(Args ...) noexcept ;
R(Args ... , ...) noexcept ;                  R(&)(Args ... , ...) noexcept ;                  R(&&)(Args ... , ...) noexcept ;                  R(*)(Args ... , ...) noexcept ;
                                                                                                                                                  
// Qualified versions (11 types )             // Qualified versions (11 types )                // Qualified versions (11 types )                 // Qualified versions (11 types )
R(Args ...) &;                                R(&)(Args ...) &;                                R(&&)(Args ...) &;                                R(*)(Args ...) &;
R(Args ...) &&;                               R(&)(Args ...) &&;                               R(&&)(Args ...) &&;                               R(*)(Args ...) &&;
R(Args ...) const ;                           R(&)(Args ...) const ;                           R(&&)(Args ...) const ;                           R(*)(Args ...) const ;
R(Args ...) const &;                          R(&)(Args ...) const &;                          R(&&)(Args ...) const &;                          R(*)(Args ...) const &;
R(Args ...) const &&;                         R(&)(Args ...) const &&;                         R(&&)(Args ...) const &&;                         R(*)(Args ...) const &&;
R(Args ...) volatile ;                        R(&)(Args ...) volatile ;                        R(&&)(Args ...) volatile ;                        R(*)(Args ...) volatile ;
R(Args ...) volatile &;                       R(&)(Args ...) volatile &;                       R(&&)(Args ...) volatile &;                       R(*)(Args ...) volatile &;
R(Args ...) volatile &&;                      R(&)(Args ...) volatile &&;                      R(&&)(Args ...) volatile &&;                      R(*)(Args ...) volatile &&;
R(Args ...) const volatile ;                  R(&)(Args ...) const volatile ;                  R(&&)(Args ...) const volatile ;                  R(*)(Args ...) const volatile ;
R(Args ...) const volatile &;                 R(&)(Args ...) const volatile &;                 R(&&)(Args ...) const volatile &;                 R(*)(Args ...) const volatile &;
R(Args ...) const volatile &&;                R(&)(Args ...) const volatile &&;                R(&&)(Args ...) const volatile &&;                R(*)(Args ...) const volatile &&;
                                                                                                                                                  
// Variadic versions (11 types )              // Variadic versions (11 types )                 // Variadic versions (11 types )                  // Variadic versions (11 types )
R(Args ... , ...) &;                          R(&)(Args ... , ...) &;                          R(&&)(Args ... , ...) &;                          R(*)(Args ... , ...) &;
R(Args ... , ...) &&;                         R(&)(Args ... , ...) &&;                         R(&&)(Args ... , ...) &&;                         R(*)(Args ... , ...) &&;
R(Args ... , ...) const ;                     R(&)(Args ... , ...) const ;                     R(&&)(Args ... , ...) const ;                     R(*)(Args ... , ...) const ;
R(Args ... , ...) const &;                    R(&)(Args ... , ...) const &;                    R(&&)(Args ... , ...) const &;                    R(*)(Args ... , ...) const &;
R(Args ... , ...) const &&;                   R(&)(Args ... , ...) const &&;                   R(&&)(Args ... , ...) const &&;                   R(*)(Args ... , ...) const &&;
R(Args ... , ...) volatile ;                  R(&)(Args ... , ...) volatile ;                  R(&&)(Args ... , ...) volatile ;                  R(*)(Args ... , ...) volatile ;
R(Args ... , ...) volatile &;                 R(&)(Args ... , ...) volatile &;                 R(&&)(Args ... , ...) volatile &;                 R(*)(Args ... , ...) volatile &;
R(Args ... , ...) volatile &&;                R(&)(Args ... , ...) volatile &&;                R(&&)(Args ... , ...) volatile &&;                R(*)(Args ... , ...) volatile &&;
R(Args ... , ...) const volatile ;            R(&)(Args ... , ...) const volatile ;            R(&&)(Args ... , ...) const volatile ;            R(*)(Args ... , ...) const volatile ;
R(Args ... , ...) const volatile &;           R(&)(Args ... , ...) const volatile &;           R(&&)(Args ... , ...) const volatile &;           R(*)(Args ... , ...) const volatile &;
R(Args ... , ...) const volatile &&;          R(&)(Args ... , ...) const volatile &&;          R(&&)(Args ... , ...) const volatile &&;          R(*)(Args ... , ...) const volatile &&;
                                                                                                                                                  
// Noexcept versions (11 types )              // Noexcept versions (11 types )                 // Noexcept versions (11 types )                  // Noexcept versions (11 types )
R(Args ...) & noexcept;                       R(&)(Args ...) & noexcept;                       R(&&)(Args ...) & noexcept;                       R(*)(Args ...) & noexcept;
R(Args ...) && noexcept;                      R(&)(Args ...) && noexcept;                      R(&&)(Args ...) && noexcept;                      R(*)(Args ...) && noexcept;
R(Args ...) const noexcept;                   R(&)(Args ...) const noexcept;                   R(&&)(Args ...) const noexcept;                   R(*)(Args ...) const noexcept;
R(Args ...) const & noexcept;                 R(&)(Args ...) const & noexcept;                 R(&&)(Args ...) const & noexcept;                 R(*)(Args ...) const & noexcept;
R(Args ...) const && noexcept;                R(&)(Args ...) const && noexcept;                R(&&)(Args ...) const && noexcept;                R(*)(Args ...) const && noexcept;
R(Args ...) volatile noexcept;                R(&)(Args ...) volatile noexcept;                R(&&)(Args ...) volatile noexcept;                R(*)(Args ...) volatile noexcept;
R(Args ...) volatile & noexcept;              R(&)(Args ...) volatile & noexcept;              R(&&)(Args ...) volatile & noexcept;              R(*)(Args ...) volatile & noexcept;
R(Args ...) volatile && noexcept;             R(&)(Args ...) volatile && noexcept;             R(&&)(Args ...) volatile && noexcept;             R(*)(Args ...) volatile && noexcept;
R(Args ...) const volatile noexcept;          R(&)(Args ...) const volatile noexcept;          R(&&)(Args ...) const volatile noexcept;          R(*)(Args ...) const volatile noexcept;
R(Args ...) const volatile & noexcept;        R(&)(Args ...) const volatile & noexcept;        R(&&)(Args ...) const volatile & noexcept;        R(*)(Args ...) const volatile & noexcept;
R(Args ...) const volatile && noexcept;       R(&)(Args ...) const volatile && noexcept;       R(&&)(Args ...) const volatile && noexcept;       R(*)(Args ...) const volatile && noexcept;
                                                                                                                                                  
// Variadic noexcept versions (11 types )     // Variadic noexcept versions (11 types )        // Variadic noexcept versions (11 types )         // Variadic noexcept versions (11 types )
R(Args ... , ...) & noexcept;                 R(&)(Args ... , ...) & noexcept;                 R(&&)(Args ... , ...) & noexcept;                 R(*)(Args ... , ...) & noexcept;
R(Args ... , ...) && noexcept;                R(&)(Args ... , ...) && noexcept;                R(&&)(Args ... , ...) && noexcept;                R(*)(Args ... , ...) && noexcept;
R(Args ... , ...) const noexcept;             R(&)(Args ... , ...) const noexcept;             R(&&)(Args ... , ...) const noexcept;             R(*)(Args ... , ...) const noexcept;
R(Args ... , ...) const & noexcept;           R(&)(Args ... , ...) const & noexcept;           R(&&)(Args ... , ...) const & noexcept;           R(*)(Args ... , ...) const & noexcept;
R(Args ... , ...) const && noexcept;          R(&)(Args ... , ...) const && noexcept;          R(&&)(Args ... , ...) const && noexcept;          R(*)(Args ... , ...) const && noexcept;
R(Args ... , ...) volatile noexcept;          R(&)(Args ... , ...) volatile noexcept;          R(&&)(Args ... , ...) volatile noexcept;          R(*)(Args ... , ...) volatile noexcept;
R(Args ... , ...) volatile & noexcept;        R(&)(Args ... , ...) volatile & noexcept;        R(&&)(Args ... , ...) volatile & noexcept;        R(*)(Args ... , ...) volatile & noexcept;
R(Args ... , ...) volatile && noexcept;       R(&)(Args ... , ...) volatile && noexcept;       R(&&)(Args ... , ...) volatile && noexcept;       R(*)(Args ... , ...) volatile && noexcept;
R(Args ... , ...) const volatile noexcept;    R(&)(Args ... , ...) const volatile noexcept;    R(&&)(Args ... , ...) const volatile noexcept;    R(*)(Args ... , ...) const volatile noexcept;
R(Args ... , ...) const volatile & noexcept;  R(&)(Args ... , ...) const volatile & noexcept;  R(&&)(Args ... , ...) const volatile & noexcept;  R(*)(Args ... , ...) const volatile & noexcept;
R(Args ... , ...) const volatile && noexcept; R(&)(Args ... , ...) const volatile && noexcept; R(&&)(Args ... , ...) const volatile && noexcept; R(*)(Args ... , ...) const volatile && noexcept;

// functions
functions: R(Args...) [48 types]
function lvalue-references: R(&)(Args...) [48 types]
function rvalue-references: R(&&)(Args...) [48 types]
function pointers: R(*)(Args...) [48 types]
function const pointers: R(* const)(Args...) [48 types]
function volatile pointers: R(* volatile)(Args...) [48 types]
function const volatile pointers: R(* const volatile)(Args...) [48 types]
function pointer lvalue-references: R(*&)(Args...) [48 types]
function const pointer lvalue-references: R(* const &)(Args...) [48 types]
function volatile pointer lvalue-references: R(* volatile &)(Args...) [48 types]
function const volatile pointer lvalue-references: R(* const volatile &)(Args...) [48 types]
function pointer rvalue-references: R(*&&)(Args...) [48 types]
function const pointer rvalue-references: R(* const &&)(Args...) [48 types]
function volatile pointer rvalue-references: R(* volatile &&)(Args...) [48 types]
function const volatile pointer rvalue-references: R(* const volatile &&)(Args...) [48 types]

// member function pointers
member function pointers: R(C::*)(Args...) [48 types]
member function const pointers: R(C::* const)(Args...) [48 types]
member function volatile pointers: R(C::* volatile)(Args...) [48 types]
member function const volatile pointers: R(C::* const volatile)(Args...) [48 types]
member function pointer lvalue-references: R(C::*&)(Args...) [48 types]
member function const pointer lvalue-references: R(C::* const &)(Args...) [48 types]
member function volatile pointer lvalue-references: R(C::* volatile &)(Args...) [48 types]
member function const volatile pointer lvalue-references: R(C::* const volatile &)(Args...) [48 types]
member function pointer rvalue-references: R(C::*&&)(Args...) [48 types]
member function const pointer rvalue-references: R(C::* const &&)(Args...) [48 types]
member function volatile pointer rvalue-references: R(C::* volatile &&)(Args...) [48 types]
member function const volatile pointer rvalue-references: R(C::* const volatile &&)(Args...) [48]

// How many types so far?
// So far 15 x 48 = 720 function types and 12 x 48 = 576 member function pointer types.

// member object pointers
member object pointers: T C::* [12 types] (T C::*, T& C::*, T&& C::*, const T C::*, ...)
member object const pointers: T C::* const [12 types]
member object volatile pointers: T C::* volatile [12 types]
member object const volatile pointers: T C::* const volatile [12 types]
member object pointer lvalue-references: T C::*& [12 types
member object const pointer lvalue-references: T C::* const & [12 types]
member object volatile pointer lvalue-references: T C::* volatile & [12 types]
member object const volatile pointer lvalue-references: T C::* const volatile & [12 types]
member object pointer rvalue-references: T C::*&& [12 types]
member object const pointer rvalue-references: T C::* const && [12 types]
member object volatile pointer rvalue-references: T C::* volatile && [12 types]
member object const volatile pointer rvalue-references: T C::* const volatile && [12 types]

// 12 x 12 = 144 member object pointer types.

closure types: lambdas, qualified lambdas, and reference to possibly qualified lambdas
class with at least one public, possibly inherited, operator() member, including cv-ref qualified versions
struct with at least one public, possibly inherited, operator() member, including cv-ref qualified versions
union with at least one public, operator() member, including cv-ref qualified versions

// 12 or 4 x 12 = 48 types depending on the counting method.

// Total
// 1500 types (720 + 576 + 144 + 48 = 1488)

// Would C++ be that simple?
// Conversion rules
double add (double x, double y) { return x + y;}
struct adder { double operator ()(double x, double y) const { return x + y ;}};
template <class T>
struct wrapper {
    // Type and value
    using type = T;
    type value;
    // Constructor
    template <class U, class = std::enable_if_t<std::is_constructible_v<T, U&&>>>
    constexpr wrapper (U&& f): value(std::forward<U>(f)) {}
    // Converter
    constexpr operator T() { return value; }
    // Call
    template <class ... Args>
    constexpr decltype(auto) call(Args &&... args) {
        return std::invoke (value, std::forward<Args>(args)...);
    }
};
// A wrapper with a conversion operator to a function pointer , reference etc ...
std::is_invocable_v <wrapper<double (*)(double, double)>, double, double>; // true
std::is_invocable_v <wrapper<double (&)(double, double)>, double, double>; // true
std::is_invocable_v <wrapper<double (* const volatile &&)(double, double)>, double, double>; // true
// A wrapper with a conversion operator to a class with an operator ()
std::is_invocable_v <wrapper<adder>, double, double>; // false
// A wrapper over a class that has an operator ()
std::is_invocable_v <decltype(& wrapper<adder>::value), wrapper<adder>>; // true
std::is_invocable_v <decltype(& wrapper<adder>::value), wrapper<adder>, double, double >; // false
std::is_invocable_v <decltype(& wrapper<adder>::call<double , double>), wrapper<adder>, double, double>; // true

Summary
A callable type can be:
a possibly cvref-qualified function, reference to it, possibly cvref-qualified pointer or pointer reference to it
a possibly cvref-qualified pointer to a possibly cvref-qualified member function, or reference to it
a possibly cvref-qualified pointer to a possibly cvref-qualified member object, or reference to it
a possibly cvref-qualified object of closure, class, struct, union type with at least 1 public operator(), or reference to it
a possibly cvref-qualified object of class, struct, union type with at least one public non-explicit conversion operator to
a possibly cvref-qualified pointer, pointer reference or reference to a possibly cvref-qualified function, or reference to it
for a total of more than 1500 forms of types
