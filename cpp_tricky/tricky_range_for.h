// https://en.cppreference.com/w/cpp/language/range-for

// If range_expression returns a temporary, its lifetime is extended until the end of the loop, as indicated by binding to the forwarding reference __range, but beware that the lifetime of any temporary within range_expression is not extended.
// 
// This problem may be worked around using init-statement:
// 
// for (auto& x : foo().items()) { /* .. */ } // undefined behavior if foo() returns by value
// for (T thing = foo(); auto& x : thing.items()) { /* ... */ } // OK
