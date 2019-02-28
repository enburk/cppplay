// Louis Brandy - Curiously Recurring C++ Bugs at Facebook [CppCon2017]

class sync_object
{
    std::mutex m_mutex;

    void do_the_mutation () { /* ... */ }

    public: void update () noexcept
    {
        std::unique_lock<std::mutex> (m_mutex); // is a declaration of shadowing local variable m_mutex, same as:
    //  std::unique_lock<std::mutex>  m_mutex ; // error C2086: 'std::unique_lock<std::mutex> m_mutex': redefinition

    //  std::lock_guard <std::mutex> (n_mutex); // error C2512: 'std::lock_guard<std::mutex>': no appropriate default constructor available
    //  std::scoped_lock<std::mutex> (n_mutex); // error C2512: 'std::scoped_lock<std::mutex>': no appropriate default constructor available

        do_the_mutation ();
    }

    void this_stuff_will_be_successfully_compile_also ()
    {
        std::string (foo);
    //  std::string {foo};  // VC 2017: warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
    //  std::string("foo"); // VC 2017: warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
    }
};

TEST_OFF
{
    // using namespace std;
    // int cin;
    // cin >> cin;           // VC 2017: warning C4552: '>>': result of expression not used
    // cout << "cin" << cin; // VC 2017: error C4700: uninitialized local variable 'cin' used
};
