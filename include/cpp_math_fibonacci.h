// Sean Parent - Better Code: Concurrency [NDC 2017]
// From Mathematics to Generic Programming [Alexander A. Stepanov, Daniel E. Rose]

template <typename N>
array<N, 4> identity_element(const multiply_2x2<N>&) { return { N(1), N(0), N(0), N(1) }; }

// Egyptian Multiplication (Russian Peasant Algorithm)
template <typename T, typename N, typename O>
T power(T x, N n, O op)
{
    if (n == 0) return identity_element(op);
    
    while ((n & 1) == 0) {
        n >>= 1;
        x = op(x, x);
    }
    
    T result = x;
    n >>= 1;
    while (n != 0) {
        x = op(x, x);
        if ((n & 1) != 0) result = op(result, x);
        n >>= 1;
    }
    return result;
}

template <typename N>
struct multiply_2x2 {
    array<N, 4> operator()(const array<N, 4>& x, const array<N, 4>& y)
    {
        return { x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3],
                 x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
    }
};

//      n
// [1 1]   = [Fn+1 Fn  ]
// [1 0]   = [Fn   Fn-1]

template <typename R, typename N>
R fibonacci(N n) {
    if (n == 0) return R(0);
    return power(array<R, 4>{ 1, 1, 1, 0 }, N(n - 1), multiply_2x2<R>())[0];
}

0.72s to calculate
208,988 digits

future<cpp_int> x = async([]{ return fibonacci<cpp_int>(1'000'000); });
// Do Something
cout << x.get() << endl;


Exception Marshalling
future<cpp_int> x = async([]{
throw runtime_error("failure");
return fibonacci<cpp_int>(1'000'000);
});
// Do Something
try {
cout << x.get() << endl;
} catch (const runtime_error& error) {
cout << error.what() << endl;
}