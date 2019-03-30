// https://www.youtube.com/watch?v=fanm5y00joc
// Alexandre Stepanov - Greatest Common Measure: The Last 2500 Years
// http://stepanovpapers.com/notes.pdf

// Pythagoreans (6th-5th century BC)
line_segment gcm(line_segment a,
                 line_segment b) {
    if (a == b)   return a;
    if (a > b)    return gcm (a-b, b);
 /* if (b > a) */ return gcm (a, b-a);
}

// Euclid (4th-3th century BC)
// Euclid guaranteed termination by changing input types
unsigned int gcd(unsigned int a,
                 unsigned int b) {
    assert (a > 0 && b > 0);
    if (a == b)   return a;
    if (a > b)    return gcd (a-b, b);
 /* if (b > a) */ return gcd (a, b-a);
}

// Leonardo Pisano (Fibonacci) (1170-1250)
unsigned int gcd(unsigned int m,
                 unsigned int n) {
    while (n != 0) {
        unsigned int t = m % n;
        m = n;
        n = t;
    }
    return m;
}

// Simon Stevin (1548–1620)
polynomial<real>
gcd(polynomial<real> m,
    polynomial<real> n) {
    while (n != 0) {
        polynomial<real> t = m % n;
        m = n;
        n = t;
    }
    return m;
}

// Karl Gauss (1777–1855)
complex<int>
gcd(complex<int> m,
    complex<int> n) {
    while (n != 0) {
        complex<int> t = m % n;
        m = n;
        n = t;
    }
    return m;
}

// Richard Dedekind, Emmy Noether, B. L. van der Waerden
template <
EuclideanRingElement >
EuclideanRingElement
gcd(EuclideanRingElement m,
    EuclideanRingElement n) {
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

// Knuth's objection: gcd (1,-1) == -1
template <
EuclideanRingElement >
EuclideanRingElement
gcd(EuclideanRingElement m,
    EuclideanRingElement n) {
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    if (m < 0) m = -m; // implies ordering
    return m;
}

// Greatest common divisor is a common divisor that
//    is divisible by any other common divisor. 
//     (there is nothing about comparing them)

template <class EuclideanDomain>
triple<EuclideanDomain, EuclideanDomain, EuclideanDomain>
extended_euclid<EuclideanDomain u, EuclideanDomain v) {
    EuclideanDomain u0 = 1;
    EuclideanDomain v0 = 0;
    EuclideanDomain u1 = u;
    EuclideanDomain v1 = v;
    while (v1 != 0) {
        EuclideanDomain q = u1/v1;
        u0 -= v0 * q;
        swap(u0, v0);
        u1 -= v1 * q;
        swap(u1, v1);
    }
    return make_triple (u0, (u1 - u*u0) / v, u1);
}

// sqrt(2) = 1 + 1/(2 + 1/(1 + 1/(2 + ...
// irrational number representation as periodical continued fractions

int gcd_modulus (int a, int b)
{
    while (true)
    {
        if (b == 0) return a; a %= b;
        if (a == 0) return b; b %= a;
    }
}
