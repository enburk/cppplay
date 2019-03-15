class Empty  {};
class Derived1 :         public Empty {}; 
class Derived2 :         public Empty { char c; }; 
class Derived3 : virtual public Empty {}; 
class Derived4 : virtual public Empty { char c; };

TEST_OFF
{
    TEST (sizeof Empty);    // VS 2017 x64: 1
    TEST (sizeof Derived1); // VS 2017 x64: 1
    TEST (sizeof Derived2); // VS 2017 x64: 1
    TEST (sizeof Derived3); // VS 2017 x64: 8
    TEST (sizeof Derived4); // VS 2017 x64: 16

    // std::max_align_t is usually synonymous with the largest scalar type, 
    // which is long double on most platforms, and its alignment requirement
    // is either 8 or 16.
    TEST (sizeof  (std::max_align_t)); // VS 2017 x86, x64: 8
    TEST (alignof (std::max_align_t)); // VS 2017 x86, x64: 8
    TEST (sizeof  (long double));      // VS 2017 x86, x64: 8
    TEST (alignof (long double));      // VS 2017 x86, x64: 8
};

// Regular std::malloc aligns memory suitable for any object type
// (which, in practice, means that it is aligned to alignof(std::max_align_t)).
size_t malloc_alignment_in_bytes (size_t n)
{
    void* p = std::malloc (n);
    size_t a = reinterpret_cast<size_t>(p);
    // std::stringstream ss; ss << std::hex << p << " " << a << " " << (a % sizeof std::max_align_t); cout << ss.str() << endl;
    size_t b = 1; while ((a & 1) == 0) { b <<= 1; a >>= 1; }
    std::free (p); return b;
}

TEST_OFF
{
    TEST (malloc_alignment_in_bytes ( 0));
    TEST (malloc_alignment_in_bytes ( 1));
    TEST (malloc_alignment_in_bytes ( 8));
    TEST (malloc_alignment_in_bytes (16));
    TEST (malloc_alignment_in_bytes (32));
    TEST (malloc_alignment_in_bytes (64));
    // Possible output:
    // malloc_alignment_in_bytes ( 0) >>> 16
    // malloc_alignment_in_bytes ( 1) >>> 16
    // malloc_alignment_in_bytes ( 8) >>> 64
    // malloc_alignment_in_bytes (16) >>> 16
    // malloc_alignment_in_bytes (32) >>> 2048
    // malloc_alignment_in_bytes (64) >>> 16
};

struct alignas(16) sse_t
{
    float sse_data[4];
};

alignas(128) char cacheline[128];
