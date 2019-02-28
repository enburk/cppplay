void signal_handler (int signal) 
{
    // The behavior is undefined if any signal handler performs any of the following:
    //  call to any library function, except ... signal-safe functions
    //  (note, in particular, dynamic allocation is not signal-safe)
    // ...
    switch (signal)
    {
        case SIGTERM : cout << "SIGTERM" << endl; break; // termination request, sent to the program
        case SIGSEGV : cout << "SIGSEGV" << endl; break; // invalid memory access (segmentation fault)
        case SIGINT  : cout << "SIGINT " << endl; break; // external interrupt, usually initiated by the user
        case SIGILL  : cout << "SIGILL " << endl; break; // invalid program image, such as invalid instruction
        case SIGABRT : cout << "SIGABRT" << endl; break; // abnormal termination condition, as is e.g. initiated by std::abort()
        case SIGFPE  : cout << "SIGFPE " << endl; break; // erroneous arithmetic operation such as divide by zero
        default : cout << "Unexpected signal " << signal << endl;
    }
    if (signal != SIGINT
    &&  signal != SIGFPE) std::_Exit(EXIT_FAILURE);
    // Causes normal program termination to occur without completely cleaning the resources.
    // Destructors of variables with automatic, thread local and static storage durations are not called.
    // Functions passed to std::at_quick_exit() or std::atexit() are not called.
    // Whether open resources such as files are closed is implementation defined.
}

template<int kind> struct Counter
{
    inline static
    std::atomic<int> N = 1;
    std::atomic<int> n = N++;
};
struct Static : Counter<1> { std::string s; Static (std::string s) : s (s) {}; ~Static () { cout << "Static " << s << " " << n << " dtor" << endl; } };
struct Local  : Counter<2> { std::string s; Local  (std::string s) : s (s) {}; ~Local  () { cout << "Local  " << s << " " << n << " dtor" << endl; } };

void slow_exit_handler () { cout << "slow exit handler" << endl; }
void fast_exit_handler () { cout << "fast exit handler" << endl; }

auto thread = [&](int n)
{
    static Static s {"fork thread"}; Local l {"fork thread"};
    
    if (std::atexit        (slow_exit_handler)) cout << "slow_exit_handler registration failed" << endl;
    
    if (std::at_quick_exit (fast_exit_handler)) cout << "fast_exit_handler registration failed" << endl;

    std::this_thread::sleep_for (500ms);        cout << "thread " << n << " finish" << endl;
};

TEST_ON
{
    static Static s {"main thread"}; Local l {"main thread"};
    
    std::thread t1 (thread, 1);
    std::thread t2 (thread, 2);

    /// std::quick_exit (1);
        // fast exit handler
        // fast exit handler
    /// std::      exit (1);
        // slow exit handler
        // slow exit handler
        // Static 2 dtor
        // Static 1 dtor
    /// std::abort ();
    /// std::raise (SIGTERM);
    /// cout << "SIGINT? " << 1/(Counter<2>::N-4) << endl;

    t1.join ();
    t2.join ();
    // thread 2 finish
    // Local  3 dtor
    // thread 1 finish
    // Local  2 dtor
    // Local  1 dtor
    // slow exit handler
    // slow exit handler
    // Static 2 dtor
    // Static 1 dtor
};