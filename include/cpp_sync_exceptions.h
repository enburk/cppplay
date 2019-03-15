// https://stackoverflow.com/questions/233127/how-can-i-propagate-exceptions-between-threads

TEST_ON
{
    std::exception_ptr e;
    std::thread thread ([&e]()
    {
        try { ::operator new (-1); } catch (...) { e = std::current_exception (); }
    });

    thread.join();

    try { std::rethrow_exception (e); } catch (const std::bad_alloc & a)
    {
        cout << "Caught: " << a.what () << endl;
    }
};
