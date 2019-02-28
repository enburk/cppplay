// http://www.stroustrup.com/terminology.pdf
// 
// i - has identity
// I - hasn't identity
// m - movable
// M - not movable
// 
// lvalue                            xvalue                           prvalue
//                                                                (pure movable)
//   iM                                im                                Im
//
//            glvalue                               rvalue
//     (generalized lvalue)            
//               i                                     m

// https://stackoverflow.com/questions/3601602/what-are-rvalues-lvalues-xvalues-glvalues-and-prvalues
//     ______ ______
//    /      X      \
//   /      / \      \
//  |   l  | x |  pr  |
//   \      \ /      /
//    \______X______/
//        gl    r
// Examples with functions:
// 
int   prvalue();
int&   lvalue();
int&&  xvalue();
// But also don't forget that named rvalue references are lvalues:
// 
void foo(int&& t) {
  // t is initialized with an rvalue expression
  // but is actually an lvalue expression itself
}

// Scott Meyers, The Universal Reference/Overloading Collision Conundrum [NWCPP 2013]

template<typename T> void f (T&& param);

TEST_OFF
{
    T v; const T c;

    f (v);           // param's type is T &
    f (c);           // param's type is const T &
    f (std::move(v); // param's type is T &&
    f (std::move(c); // param's type is const T &&
}
