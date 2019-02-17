// CppCon 2016: Chandler Carruth “Garbage In, Garbage Out: Arguing about Undefined Behavior..."

bool mainGtU (uint32_t i1, uint32_t i2, uint8_t* block) // 32-bit arithmetic on 64-bit machine
{
    uint8_t c1, c2;

    /* 1 */
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    /* 2 */
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    // ...

    return false;
}

// >    1. # BB#2: # %if.end
// >    2.  leal 1(%rdi), %eax
// >    3.  leal 1(%rsi), %ebp
// >    4.  movb (%rdx,%rax), %al
// >    5.  movb (%rdx,%rbp), %bl
// >    6.  cmpb %bl, %al
// >    7.  jne .LBB27_1

// VC 2017 x64:

//  ; 7    :     /* 1 */
//  	movzx	ecx, BYTE PTR [r8+1]
//  	movzx	eax, BYTE PTR [r8+2]
//  	cmp	cl, al
//  	je	SHORT $LN2@mainGtU
//  ; 11   :     /* 2 */
//  	movzx	ecx, BYTE PTR [r8+3]
//  	cmp	al, cl
//  	jne	SHORT $LN5@mainGtU

bool mainGtI (int32_t i1, int32_t i2, uint8_t* block) // int32_t insted of uint32_t
{
    uint8_t c1, c2;

    /* 1 */
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    /* 2 */
    c1 = block[i1]; c2 = block[i2];
    if (c1 != c2) return (c1 > c2);
    i1++; i2++;
    // ...

    return false;
}

// >    1. # BB#2: # %if.end
// >    2.  movzbl 1(%rdx,%rsi), %eax
// >    3.  cmpb %al, 1(%rdx,%rdi)
// >    4.  jne .LBB27_1

// VC 2017 x86:

//  ; 22   :     /* 1 */
//  	mov	ecx, DWORD PTR _block$[ebp]
//  	mov	dl, BYTE PTR [ecx+1]
//  	mov	al, BYTE PTR [ecx+2]
//  	cmp	dl, al
//  	je	SHORT $LN2@mainGtI
//  ; 26   :     /* 2 */
//  	mov	cl, BYTE PTR [ecx+3]
//  	cmp	al, cl
//  	jne	SHORT $LN5@mainGtI

TEST_ON
{
    std::vector<uint8_t> v (256); std::iota (v.begin (), v.end (), 0);

    int N = 1'000'000; bool b1 = true, b2 = true;

    Time t0; for (int i=0; i<N; i++) b1 ^= mainGtU (1, 2, &v [0]);
    Time t1; for (int i=0; i<N; i++) b2 ^= mainGtI (1, 2, &v [0]);
    Time t2;

    cout << "mainGtU " << b1 << " : " << t1-t0 << " sec" << endl;
    cout << "mainGtI " << b2 << " : " << t2-t1 << " sec" << endl;
};