// CppCon 2018: JF Bastien “Signed integers are two's complement”

// signed integer overflow is undefined behaviour
template <typename Int>
bool overflow1 (Int lhs, Int rhs) noexcept {
    return (lhs + rhs) < lhs;
}
//   X86:                           ARM:
//   shrl    $31, %esi              lsr     w0, w1, #31
//   movl    %esi, %eax             ret
//   retq

// "correct" way:
template <typename Int>
bool overflow2 (Int lhs, Int rhs) noexcept {
#pragma warning(push)
#pragma warning(disable: 4018) // '<': signed/unsigned mismatch
#pragma warning(disable: 26493) // Don't use C-style casts
    using U = std::make_unsigned_t<Int>;
    return (U(lhs) + U(rhs)) < lhs;
#pragma warning(pop)
}
//   X86:                           ARM:
//   addl    %edi, %esi             cmn     w1, w0
//   setb    %al                    cset    w0, hs
//   ret                            ret

template <typename Int>
bool overflow3 (Int lhs, Int rhs) noexcept {
    constexpr auto max = std::numeric_limits<decltype(lhs)>::max();
    return lhs > max - rhs;
}
//   X86:                           ARM:
//   movl    $2147483647, %eax      orr     w8, wzr, 0x7fffffff
//   subl    $esi, %eax             sub     w8, w8, w1
//   cmpl    $edi, %eax             cmp     w8, w0
//   setb    %al                    cset    w0, lt
//   ret                            ret

// bitfield ovewflow is implementation defined
inline
bool overflow4 (int lhs, int rhs) noexcept {
    struct S { S(int a) noexcept : a(a) {}; long long a : 32; } s(lhs);
    return ((s.a += rhs), s.a) < lhs;
}
// GCC, MSVC - defined behaviour, clang - undefined

// checking for negative overflow as well:
template <typename Int>
bool overflow5 (Int lhs, Int rhs) noexcept {
    constexpr auto min = std::numeric_limits<decltype(lhs)>::min();
    constexpr auto max = std::numeric_limits<decltype(lhs)>::max();
    return (rhs < 0)
      ? (lhs < min - rhs)
      : (lhs > max - rhs);
}
//   X86:                           ARM:
//   testl   %esi, %esi             tbnz    w1, #31, .LBB0_2
//   js      .LBB0_1                orr     w8, wzr, 0x7fffffff
//   movl    $2147483647, %eax      sub     w8, w8, w1
//   subl    $esi, %eax             cmp     w8, w0
//   cmpl    $edi, %eax             cset    w0, lt
//   setb    %al                    ret
//   ret                          .LBB0_2:  
// .LBB0_1:                         mov     w8, #-2147483647
//   movslq  %edi, %rax             sub     w8, w8, w1, sxtw
//   movslq  %esi, %rcx             cmp     w8, w0, sxtw
//   movq    $-2147483647, %rdx     cset    w0, gt
//   subq    $rcx, %rdx             ret
//   cmpq    $rax, %rdx             
//   setq    %al                    
//   ret                            

#if defined(_MSC_VER)
#if defined(_M_IX86) || defined(_M_X64)
#pragma optimize("", off) // force __readeflags be after assignment
bool __builtin_add_overflow(int lhs, int rhs, int* tmp) noexcept {
    if (tmp) *tmp = lhs + rhs; return __readeflags() & 0x801;
}
#pragma optimize("", on)
#endif
#endif

// checking for negative overflow as well:
template <typename Int>
bool overflow6 (Int lhs, Int rhs) noexcept {
    Int tmp;
    return __builtin_add_overflow(lhs, rhs, &tmp);
}
//   X86:                           ARM:
//   addl    %esi, %edi             cmn     w0, w1
//   seto    %al                    cset    w0, vs
//   retq                           ret

TEST_OFF
{
    constexpr int min = std::numeric_limits<int>::min();
    constexpr int max = std::numeric_limits<int>::max();

    TEST (overflow1 (min,-1)); TEST (overflow1 (max, 1)); cout << endl;
    TEST (overflow2 (min,-1)); TEST (overflow2 (max, 1)); cout << endl;
    TEST (overflow3 (min,-1)); TEST (overflow3 (max, 1)); cout << endl;
    TEST (overflow4 (min,-1)); TEST (overflow4 (max, 1)); cout << endl;
    TEST (overflow5 (min,-1)); TEST (overflow5 (max, 1)); cout << endl;
    TEST (overflow6 (min,-1)); TEST (overflow6 (max, 1)); cout << endl;

    // undefined behaviour can cause
    // not only addition, subtraction and multiplication:
    auto one = std::lround(std::sqrt(4)/2); // workaround warning
    TEST (min/-one);
    TEST (abs(min));

    // Output:
    // 
    // overflow1 (min,-1) >>> 0
    // overflow1 (max, 1) >>> 1
    // 
    // overflow2 (min,-1) >>> 1
    // overflow2 (max, 1) >>> 0
    // 
    // overflow3 (min,-1) >>> 0
    // overflow3 (max, 1) >>> 1
    // 
    // overflow4 (min,-1) >>> 0
    // overflow4 (max, 1) >>> 1
    // 
    // overflow5 (min,-1) >>> 1
    // overflow5 (max, 1) >>> 1
    // 
    // overflow6 (min,-1) >>> 1
    // overflow6 (max, 1) >>> 1
    // 
    // min/-one >>> -2147483648
    // abs(min) >>> -2147483648
};

// Kotaku, Why Gandhi Is Such An Asshole In Civilization
// https://kotaku.com/why-gandhi-is-such-an-asshole-in-civilization-1653818245

// Donald Knuth. The Art of Computer Programming, Volume 2
//  Detail-oriented readers and copy editors should notice the position of the apostrophe
//  in terms like “two’s complement” and “ones’ complement”:
//  A two’s complement number is complemented with respect to a single power of 2,
//  while a ones’ complement number is complemented with respect to a long sequence of 1s.
//  Indeed, there is also a “twos’ complement notation,” which has radix 3
//  and complementation with respect to (2 . . . 22)3.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r3.html

// Krister Walfridsson's blog: How undefined signed overflow enables optimizations in GCC
// https://kristerw.blogspot.com/2016/02/how-undefined-signed-overflow-enables.html
// 
// (x * c) cmp 0   ->   x cmp 0 
// (x * c1) / c2   ->   x * (c1 / c2) if c1 is divisible by c2
// (-x) / (-y)     ->   x / y
// x + c < x       ->   false
// x + c <= x      ->   false
// x + c > x       ->   true
// x + c >= x      ->   true
// (-x) cmp (-y)   ->   y cmp x
// x + c > y       ->   x + (c - 1) >= y
// x + c <= y      ->   x + (c - 1) < y
// (x + c1) cmp c2         ->   x cmp (c2 - c1)
// (x + c1) cmp (y + c2)   ->   x cmp (y + (c2 - c1)) if c1 <= c2
// 
// Changing comparisons x<y to true or false if the ranges for x and y does not overlap
// Changing min(x,y) or max(x,y) to x or y if the ranges do not overlap
// Changing abs(x) to x or -x if the range does not cross 0
// Changing x/c to x>>log2(c) if x>0 and the constant c is a power of 2
// Changing x%c to x&(c-1) if x>0 and the constant c is a power of 2
