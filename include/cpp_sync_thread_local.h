namespace sync_thread_local // https://en.cppreference.com/w/cpp/language/storage_duration
{
    std::mutex mutex;

    std::condition_variable cv;

    thread_local int counter = 0;

    int& singleton () { thread_local int Counter = 0; return Counter; } // thread_local implies static

    std::unordered_map<std::string, std::string*> smap; 
 
    void increase (const std::string & thread_name)
    {
        ++counter; // modifying outside a lock is okay; this is a thread-local variable
        ++singleton ();
        std::unique_lock<std::mutex> lock (mutex);
        cout << "counter for " << thread_name << ": " << counter << endl;
        cout << "singleton for " << thread_name << ": " << singleton () << endl;

        thread_local std::string s; smap [thread_name] = &s; cv.notify_all ();

        cout << "thread local string address published (thread " << thread_name << ")" << endl;

        while (*smap [thread_name] != thread_name) cv.wait (lock);

        cout << "thread local string updated externally (thread " << thread_name << ")" << endl;
    }
 
    TEST_OFF
    {
        std::thread a (increase, "a");
        std::thread b (increase, "b");
        {
            std::unique_lock lock (mutex); // <std::mutex>
            while (smap.size () != 2) cv.wait (lock);
            *smap ["a"] = "a";
            *smap ["b"] = "b";
            cv.notify_all ();
        }
        a.join();
        b.join();

        cout << "counter for main: " << counter << endl;
        cout << "singleton for main: " << singleton () << endl;
    };
    // Possible output:
    //
    // counter for a: 1
    // singleton for a: 1
    // thread local string address published (thread a)
    // counter for b: 1
    // singleton for b: 1
    // thread local string address published (thread b)
    // thread local string updated externally (thread b)
    // thread local string updated externally (thread a)
    // counter for main: 0
    // singleton for main: 0
}