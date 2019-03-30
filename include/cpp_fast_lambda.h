// Nir Friedman “Understanding Optimizers: Helping the Compiler Help You” [CppCon 2018]

bool compare (int x, int y) { return x > y; }

TEST_OFF
{
    std::vector<int> v0 (1'000'000); for (int & n : v0) n = randint ();
    std::vector<int> v1 = v0;

    Time t0; std::sort (v0.begin(), v0.end(), compare); // function pointers rarely get inlined
    Time t1; std::sort (v1.begin(), v1.end(), [] (int x, int y) { return compare (x, y); });
    Time t2;

    cout << "compare pointer: " << t1-t0 << " sec" << endl;
    cout << "compare lambda : " << t2-t1 << " sec" << endl;
};
//  Possible output:
//  compare pointer: 0.118'510'974 sec
//  compare lambda : 0.084'272'873 sec