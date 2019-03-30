TEST_ON
{
    oops ( element e        ;     ), { "ctor: _", "-------", "dtor: _" });
//  oops ( element e   ()   ;     ), {            "-------"            }); // it's a function!
    oops ( element e   {}   ;     ), { "ctor: _", "-------", "dtor: _" });
    oops ( element e = {}   ;     ), { "ctor: _", "-------", "dtor: _" });
    oops ( element e   ('a');     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e   {'a'};     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e = ('a');     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e = {'a'};     ), { "ctor: a", "-------", "dtor: a" });
    oops ( element e =  'a' ;     ), { "ctor: a", "-------", "dtor: a" });
    oops ( auto e = element();    ), { "ctor: _", "-------", "dtor: _" });
    oops ( auto e = element{};    ), { "ctor: _", "-------", "dtor: _" });
    oops ( auto e = element('a'); ), { "ctor: a", "-------", "dtor: a" });
    oops ( auto e = element{'a'}; ), { "ctor: a", "-------", "dtor: a" });

// CppCon 2018: Nicolai Josuttis “The Nightmare of Initialization in C++”

    int  i01; /* undefined value */ i01 = 42; // prevent warning
    int  i02 = 42;   
    int  i03 (42);    
    int  i04 = int();
    
    int  i05 {42};    
    int  i06 {};      
    int  i07 = {42};   
    int  i08 = {};   
    
    auto i09  = 42;  // int
    auto i10   {42}; // int (C++14), std::initializer_list<int> (C++11)
    auto i11 = {42}; static_assert(std::is_same_v<decltype(i11), std::initializer_list<int>>);
    auto i12 = int{42}; // int
    
    int  i13 (); // declares a function

//  int  i14   (7,9);    // compile-time error
    int  i15 = (7,9);    // OK, 9 (comma operator)
//  int  i16 = int(7,9); // compile-time error
//  auto i17   (7,9);    // compile-time error
    auto i18 = (7,9);    // OK, int 9 (comma operator);
//  auto i19 = int(7,9); // compile-time error;

};

/*
struct A {
    int i;
};

int main() {
    A a = {.i = 0}; // C designated initializers aren’t supported in C++ until C++20
    std::cout << a.i << std::endl;
}*/
