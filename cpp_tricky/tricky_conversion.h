// CppCon 2018: Nicolai Josuttis “The Nightmare of Initialization in C++”

struct Explicit
{
    explicit
    Explicit (int n) noexcept {}
};
struct Implicit
{
    Implicit (int n) noexcept {}
};

TEST_ON
{
    const auto exfoo = [](const Explicit &) noexcept {};
    const auto imfoo = [](const Implicit &) noexcept {};

//  exfoo (42); // error
    exfoo (Explicit(42));
    exfoo (Explicit{42});

    imfoo (42);
    imfoo (Implicit(42));
    imfoo (Implicit{42});

    Explicit ex1   (42);
    Explicit ex2   {42};
//  Explicit ex3 =  42;  // error C2440: 'initializing': cannot convert from 'int' to 'Explicit'
//  Explicit ex4 = {42}; // error C3445: copy-list-initialization of 'Explicit' cannot use an explicit constructor
    Explicit ex5 = Explicit(42);
    Explicit ex6 = Explicit{42};

    Implicit im1   (42);
    Implicit im2   {42};
    Implicit im3 =  42;
    Implicit im4 = {42};
    Implicit im5 = Implicit(42);
    Implicit im6 = Implicit{42};
};

