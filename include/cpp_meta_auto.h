// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0127r1.html

template <auto ... vs> struct HeterogenousValueList {};
using MyList1 = HeterogenousValueList<42, 'X', 13u>;

template <auto v0, decltype(v0) ... vs> struct HomogenousValueList {};
using MyList2 = HomogenousValueList<1, 2, 3>;


template<auto Func>
struct FuncWrapper final
{
    template<typename... Args>
    auto operator()(Args &&... args) const
    {
        return Func(std::forward<Args>(args)...);
    }
};



// log each argument in a comma-separated list
template <class... Args> void log_args(Args... args);

// struct template that accepts a function pointer and a name
template <auto f, const char* name> struct fn_logger;

// use partial specialization to constrain f
// note that a second specialization would be needed to support functions returning void
template <class R, class... Args, R (* f)(Args...), const char* name>
struct fn_logger<f, name>
{
    // call f, logging arguments and result
    static R call(Args... args)
    {
        cout << name << '(';
        log_args(args...);
        cout << ')';
        auto result = f(args...);
        cout << " -> " << result << endl;
        return result;
    }
};

// variable template to simplify use of fn_logger
template <auto f, const char* name> constexpr auto logger = fn_logger<f, name>::call;
The function type no longer needs to be explicitly specified, which means the instantiation can go back to the desired form:

library::register_callback(logger<my_callback, cbname>);