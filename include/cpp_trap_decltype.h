// https://stackoverflow.com/questions/4762662/are-parentheses-around-the-result-significant-in-a-return-statement

// If the argument is either the unparenthesised name of an object/function, 
// or is a member access expression (object.member or pointer->member), 
// then the decltype specifies the declared type of the entity specified by this expression.
// 
// If the argument is any other expression of type T, then
// 
// a) if the value category of expression is xvalue, then the decltype specifies T&&
// 
// b) if the value category of expression is lvalue, then the decltype specifies T&
// 
// c) otherwise, decltype specifies T

struct A
{
    element e;

    decltype(auto) func1 () { return e;  } // return type is element
    decltype(auto) func2 () { return(e); } // return type is element&

    decltype(auto) func3 () { e = 'a'; return e;  } // return type is element
    decltype(auto) func4 () { e = 'b'; return(e); } // return type is element&
};


TEST_OFF
{
//  element & e1 = A ().func1 (); // compile error
    element & e2 = A ().func2 (); // compiles successfully

    A a1 {'a'}; auto b1 = a1.func1 (); b1 = 'b'; cout << a1.e.s << endl;
    A a2 {'a'}; auto b2 = a2.func2 (); b2 = 'b'; cout << a2.e.s << endl; // b2 : element

    log::print ();

    element ee1 = a1.func3 ();
    element ee2 = a1.func4 ();

    log::print ();

};
