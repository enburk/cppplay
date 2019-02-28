namespace inheritance1
{
    struct B { int n;  B (int n=0) : n(n) { cout << "B " << n << endl; } };
    struct X : virtual B { X()     : B(1) { cout << "X " << 1 << endl; } };
    struct Y : virtual B { Y()     : B(2) { cout << "Y " << 2 << endl; } };
    struct C : X, Y      { C()     : B(3), X(), Y() { cout << "C" << endl; } };
    struct D : X, Y      { D()     :       X(), Y() { cout << "D" << endl; } };
    struct E : X, Y      { E()                      { cout << "E" << endl; } };

    TEST_OFF { C c; D d; E e; };
    // B 3
    // X 1 // do not call the constructor of B because B is a virtual base
    // Y 2 // do not call the constructor of B because B is a virtual base
    // C
    // B 0
    // X 1
    // Y 2
    // D
    // B 0
    // X 1
    // Y 2
    // E
}
namespace inheritance2
{
    struct B { int n;  B (int n=0) : n(n) { cout << "B " << n << endl; } };
    struct X :         B { X()     : B(1) { cout << "X " << 1 << endl; } };
    struct Y :         B { Y()     : B(2) { cout << "Y " << 2 << endl; } };
//  struct C : X, Y      { C()     : B(3), X(), Y() { cout << "C" << endl; } }; // error C2385: ambiguous access of 'B'
//  struct C : X, Y      { C()  : X::B(3), X(), Y() { cout << "C" << endl; } }; // illegal member initialization: 'B' is not a base or member
    struct D : X, Y      { D()     :       X(), Y() { cout << "D" << endl; } };
    struct E : X, Y      { E()                      { cout << "E" << endl; } };

    TEST_OFF { D d; E e; };
    // B 1
    // X 1
    // B 2
    // Y 2
    // D
    // B 1
    // X 1
    // B 2
    // Y 2
    // E
}
namespace inheritance3
{
    struct A     { virtual std::string pure () = 0 { return std::string ("A"); }; };
    struct B : A { virtual std::string pure ()     { return std::string ("B") + A::pure () + this->A::pure (); } };

    TEST_OFF { cout << B ().pure () << endl; };
    // BAA
}
