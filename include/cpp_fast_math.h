TEST_ON
{
    const int n1 = 1'000;
    const int n2 = 1'000'000'000;

    Time t0; int64_t sum1 = 0; for (int i=0; i<=n1; i++) sum1 += i;
    Time t1; int64_t sum2 = 0; for (int i=0; i<=n2; i++) sum2 += i;
    Time t2;

    cout << "sum1 : " << t1-t0 << " sec" << endl;
    cout << "sum2 : " << t2-t1 << " sec" << endl;

    assert (sum1 == (int64_t) n1*(n1+1)/2); // some compilers know this formula
    assert (sum2 == (int64_t) n2*(n2+1)/2); // some compilers know this formula
};
//  Possible output:
//  sum1 : 0.000'000'000 sec
//  sum2 : 0.000'000'000 sec
//  or (Visual C++ 2017   00369-60000-00001-AA363):
//  sum1 : 0.000'000'410 sec
//  sum2 : 0.310'228'077 sec

