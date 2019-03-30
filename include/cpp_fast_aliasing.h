// Undefined Behavior in 2017 by John Regehr [CppCon 2017]

int check(int *h, int *k) {
    *h = 5;
    *k = 6;
    return *h;
}
// check:
//     movl $5, (%rdi)
//     movl $6, (%rsi)
//     movl (%rdi), %eax ; maybe h == k ?
//     retq

int check(int *h, long *k) {
    *h = 5;
    *k = 6;
    return *h;
}
// check:
//     movl $5, (%rdi)
//     movq $6, (%rsi)
//     movl $5, %eax
//     retq

// Optimization is valid even when sizeof(int) == sizeof(long)
// Optimization is not valid for int * and unsigned *

// Type Punning In C++17 - Avoiding Pun-defined Behavior by Scott Schurr [CppCon 2017]
int sum_twice (int* a, int* b) {
    *a += *b;
    *a += *b;
    return *a;
}
TEST_ON
{
    int c[] {2, 2}; // a and b are different
    cout << sum_twice (&c[0], &c[1]) << endl; // 6
    int d {2}; // a and b are the same
    cout << sum_twice (&d , &d ) << endl; // 8
};

// https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8

// Both si and ui are signed or unsigned types corresponding to each others dynamic types
// We can see from this godbolt(https://godbolt.org/g/KowGXB) the optimizer assumes aliasing.
signed int foo( signed int &si, unsigned int &ui ) {
  si = 1;
  ui = 2;

  return si;
}

(11.6) — an aggregate or union type that includes one of the aforementioned types among its elements or nonstatic data members (including, recursively, an element or non-static data member of a subaggregate or contained union),

struct foo {
 int x;
};

// Compiler Explorer example(https://godbolt.org/g/z2wJTC) shows aliasing assumption
int foobar( foo &fp, int &ip ) {
 fp.x = 1;
 ip = 2;

 return fp.x;
}

foo f; 
foobar( f, f.x ); 


(11.7) — a type that is a (possibly cv-qualified) base class type of the dynamic type of the object,

struct foo { int x ; };

struct bar : public foo {};

int foobar( foo &f, bar &b ) {
  f.x = 1;
  b.x = 2;

  return f.x;
}

(11.8) — a char, unsigned char, or std::byte type.

int foo( std::byte &b, uint32_t &ui ) {
  b = static_cast<std::byte>('a');
  ui = 0xFFFFFFFF;                   
  
  return std::to_integer<int>( b );  // b gives us a glvalue expression of type std::byte which can alias
                                     // an object of type uint32_t
}


