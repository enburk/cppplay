// https://en.cppreference.com/w/cpp/language/structured_binding

// Case 1: binding an array

int a[2] = {1,2};
 
auto [x,y] = a; // creates e[2], copies a into e, then x refers to e[0], y refers to e[1]
auto& [xr, yr] = a; // xr refers to a[0], yr refers to a[1]

// Case 2: binding a tuple-like type

float x{};
char  y{};
int   z{};
 
std::tuple<float&,char&&,int> tpl(x,std::move(y),z);
const auto& [a,b,c] = tpl;
// a names a structured binding that refers to x; decltype(a) is float&
// b names a structured binding that refers to y; decltype(b) is char&&
// c names a structured binding that refers to the 3rd element of tpl; decltype(c) is const int

// Case 3: binding to data members

struct S {
    int x1 : 2;
    volatile double y1;
};
S f();
 
const auto [x, y] = f(); // x is a const int lvalue identifying the 2-bit bit field
                         // y is a const volatile double lvalue