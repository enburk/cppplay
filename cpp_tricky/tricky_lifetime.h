TEST_ON
{
    struct A { ~A(){ cout << "~A" << endl; } };
    struct B { ~B(){ cout << "~B" << endl; } };

    auto a = [](){ static A a; };
    auto b = [](){ static B b; };

    a(); std::atexit([](){ cout << "after ~A" << endl; });
    b();
};

