struct Test
{
    // Based on:
    // Sean Parent - Inheritance is the base class of evil [GN 2013]
    // Sean Parent - Better Code: Runtime Polymorphism [NDC 2017]
    template<class type> Test (type data) : self (std::make_shared<model<type>>(std::move (data))) {}

    std::vector<int> sample() const { return self->sample(); }
    std::string description() const { return self->description(); }

    private: struct concept
    {
        virtual ~concept() = default;
        virtual std::vector<int> sample() const = 0;
        virtual std::string description() const = 0;
    };
    template<class type> struct model : public concept
    {
        type data;
        model (type data) : data (std::move (data)) {}
        std::vector<int> sample() const override { return data.sample(); }
        std::string description() const override { return data.description(); }
    };

    std::shared_ptr<const concept> self;
};


template <class F, class... Args> struct TestFunction
{
    F f;
    std::string s;
    std::tuple<Args...> args;

    TestFunction (F f, std::tuple<Args...> args, std::string s) : f (f), args (args), s (s) {}

    std::vector<int> sample () const { return std::apply (f, args); }
    std::string description () const { return s; }
};

inline auto random = []
(
    int n,
    int l = std::numeric_limits<int>::min (),
    int u = std::numeric_limits<int>::max ()
)
{
    std::vector<int> v (n);
    for (int & n : v) n = randint (l, u);
    return v;
};

inline auto nearly_sorted = [] (int n)
{
    std::vector<int> v (n);
    std::iota (v.begin(), v.end(), 0);
    int c = (int) std::log (v.size ());
    while (c-- > 0) std::swap (
        v [randint (0, n-1)],
        v [randint (0, n-1)]);
    return v;
};

TEST_ON
{
    std::vector<Test> tests;
    tests.emplace_back (TestFunction (random, std::tuple{5}, "random"));
    tests.emplace_back (TestFunction (random, std::tuple{30, 0, 5}, "few unique"));
    tests.emplace_back (TestFunction (nearly_sorted, std::tuple{25}, "nearly sorted"));

    for (auto test : tests)
    {
        cout << test.description () << endl;
        for (int n : test.sample ()) cout << n << " ";
        cout << endl << endl;
    }
};

// Possible output:
// 
// random
// 1006811233 1168850893 1205620418 -1570519362 -1789997688
// 
// few unique
// 1 1 0 5 3 1 3 3 0 3 4 2 3 1 0 5 5 1 0 1 5 4 0 4 5 5 3 3 2 2
// 
// nearly sorted
// 22 1 2 3 4 20 6 7 8 9 0 11 12 13 14 15 16 17 18 19 5 21 10 23 24
