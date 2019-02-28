class Empty  {};
class Derived1 :         public Empty {}; 
class Derived2 :         public Empty { char c; }; 
class Derived3 : virtual public Empty {}; 
class Derived4 : virtual public Empty { char c; };

TEST_ON
{
    TEST (sizeof (Empty));    // VS 2017 x64: 1
    TEST (sizeof (Derived1)); // VS 2017 x64: 1
    TEST (sizeof (Derived2)); // VS 2017 x64: 1
    TEST (sizeof (Derived3)); // VS 2017 x64: 8
    TEST (sizeof (Derived4)); // VS 2017 x64: 16

    // std::max_align_t is usually synonymous with the largest scalar type, 
    // which is long double on most platforms, and its alignment requirement
    // is either 8 or 16.
    TEST (sizeof  (std::max_align_t)); // VS 2017 x86 and x64: 8
    TEST (alignof (std::max_align_t)); // VS 2017 x86 and x64: 8
    TEST (sizeof  (long double));      // VS 2017 x86 and x64: 8
    TEST (alignof (long double));      // VS 2017 x86 and x64: 8
};
