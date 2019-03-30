struct polymorphic
{
    // prevent VS 2017 warning C26432:
    // If you define or delete any default operation in the type, define or delete them all

    virtual
   ~polymorphic              (                       ) = default;
    polymorphic              (                       ) = default;
    polymorphic              (const polymorphic  & e ) = default;
    polymorphic              (      polymorphic && e ) = default;
    polymorphic & operator = (const polymorphic  & e ) = default;
    polymorphic & operator = (      polymorphic && e ) = default;
};

template <class Sample> struct Test
{
    // Based on:
    // Sean Parent - Inheritance is the base class of evil [GN 2013]
    // Sean Parent - Better Code: Runtime Polymorphism [NDC 2017]

    typedef std::string string;

    Sample sample_data() const { return self->sample_data(); }
    string description() const { return self->description(); }

    template<class type> Test (type data) : self (std::make_shared<model<type>>(std::move (data))) {}

    private: struct concept : polymorphic
    {
        virtual Sample sample_data() const = 0;
        virtual string description() const = 0;
    };
    private: template<class type> struct model : concept
    {
        type data; model (type data) : data (std::move (data)) {}
        Sample sample_data() const override { return data.sample_data(); }
        string description() const override { return data.description(); }
    };
    private: std::shared_ptr<const concept> self;

    public: template <class F, class... Args> struct Function
    {
        F f;
        string s;
        std::tuple<Args...> args;

        Function (F f, std::tuple<Args...> args, string s) : f (f), args (args), s (s) {}

        Sample sample_data () const { return std::apply (f, args); }
        string description () const { return s; }
    };
};

template <typename T> std::ostream & operator << (std::ostream & out, const std::vector<T> & v)
{
    out << '[';
    if (!v.empty()) std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    if (!v.empty()) out << "\b\b";
    out << "]";
    return out;
}

TEST_ON
{
    using Letters = std::vector<char>;
    using Strings = std::vector<std::string>;
    using Numbers = std::vector<int>;

    const auto letters = [](int n){ return n == 0 ? Letters {'a', 'b', 'c'} : Letters {'A', 'B', 'C' }; };
    const auto strings = [](int n){ return n == 0 ? Strings {"abc", "def" } : Strings {"Abc", "Def"  }; };
    const auto numbers = [](int n){ return n == 0 ? Numbers {0, 1, 2, 3, 4} : Numbers {10, 11, 12, 13}; };

    auto tests = std::tuple
    {
        Test<Letters>::Function (letters, std::tuple{0}, "letters:"),
        Test<Letters>::Function (letters, std::tuple{1}, "letters:"),
        Test<Strings>::Function (strings, std::tuple{0}, "strings:"),
        Test<Strings>::Function (strings, std::tuple{1}, "strings:"),
        Test<Numbers>::Function (numbers, std::tuple{0}, "numbers:"),
        Test<Numbers>::Function (numbers, std::tuple{1}, "numbers:")
    };

    std::apply([](auto&&... test)
    {
        ((
            cout << test.description () << endl
                 << test.sample_data () << endl
                 << endl
        )
        , ...);
    }
    , tests);
};

