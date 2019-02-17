#include "aux_aux.h"
#include <numeric>  

namespace algo
{

// Euclidean algorithm, O (N²)

template<class M, class N> constexpr std::common_type_t<M,N> gcd (M m, N n)
{
    return n == 0 ? m : gcd (n, m - n*(m/n)); // greatest common divisor
}
template<class M, class N> constexpr std::common_type_t<M,N> lcm (M m, N n)
{
    return m == 0 || n == 0 ? 0 : m / gcd (m, n) * n; // least common multiple
}

constexpr int64_t gcd (int64_t m, int64_t n) { return n == 0           ? m :     gcd (n, m % n); }
constexpr int64_t lcm (int64_t m, int64_t n) { return m == 0 || n == 0 ? 0 : m / gcd (m, n) * n; }

template<class M, class N, class... L> constexpr std::common_type_t<M,N> gcd (M m, N n, L... more) { return gcd (gcd (m, n), more...); }
template<class M, class N, class... L> constexpr std::common_type_t<M,N> lcm (M m, N n, L... more) { return lcm (lcm (m, n), more...); }

TEST_OFF
{
    TEST (lcm (2, 3, 4, 5, 7));
    TEST (gcd (2, 420));
    TEST (gcd (3, 420));
    TEST (gcd (4, 420));
    TEST (gcd (5, 420));
    TEST (gcd (7, 420));
    cout << endl;

    const int N = 100'000;

    int imin = -N; //(int) std::sqrt (std::numeric_limits<int>::min ());
    int imax = N; //(int) std::sqrt (std::numeric_limits<int>::max ());

    std::vector<int> mm (N); for (int i=0; i<N; i++) mm [i] = randint (imin , imax);
    std::vector<int> nn (N); for (int i=0; i<N; i++) nn [i] = randint (imin , imax);
    std::vector<int> v1 (N), v3 (N);
    std::vector<int> v2 (N), v4 (N);

    Time t0; for (int i=0; i<N; i++) v1 [i] = (int) gcd (mm [i], nn [i]);
    Time t1; for (int i=0; i<N; i++) v2 [i] = std:: gcd (mm [i], nn [i]);
    Time t2; for (int i=0; i<N; i++) v3 [i] = (int) lcm (mm [i], nn [i]);
    Time t3; for (int i=0; i<N; i++) v4 [i] = std:: lcm (mm [i], nn [i]);
    Time t4;

    cout << "naive gcd " <<  t1-t0 << " sec" << endl;
    cout << "std:: gcd " <<  t2-t1 << " sec" << endl;
    cout << "naive lcm " <<  t3-t2 << " sec" << endl;
    cout << "std:: lcm " <<  t4-t3 << " sec" << endl;

    assert (v1 == v2);
    assert (v3 == v4);

    constexpr int m = std::numeric_limits<int>::min ();
    constexpr int M = std::numeric_limits<int>::max ();

    std::vector<int> extreme_m {0, 0, 0, m, m, m, M, M, M};
    std::vector<int> extreme_n {0, m, M, 0, m, M, 0, m, M};

    for (int i=0; i<9; i++)
    {
        int m = extreme_m [i]; int n = extreme_n [i];

        cout << "gcd (" << m << ", " << n << ") = ";
        cout <<  gcd ((int64_t) m, (int64_t) n) << " (int64_t) ";
        cout <<  std::gcd (m, n) << " (std) ";
        cout <<  gcd (m, n) << " (naive) " << endl;

        cout << "lcm (" << m << ", " << n << ") = ";
        cout <<  lcm ((int64_t) m, (int64_t) n) << " (int64_t) ";
        cout <<  std::lcm (m, n) << " (std) ";
        cout <<  lcm (m, n) << " (naive) " << endl;
    }

};

// Possible output:
// 
// cout << lcm (2, 3, 4, 5, 7) >>> 420
// cout << gcd (2, 420) >>> 2
// cout << gcd (3, 420) >>> 3
// cout << gcd (4, 420) >>> 4
// cout << gcd (5, 420) >>> 5
// cout << gcd (7, 420) >>> 7
// 
//      gcd 0.092'501'451 sec
// std::gcd 0.141'634'418 sec
//      lcm 0.094'193'314 sec
// std::lcm 0.127'293'620 sec

}