template<typename T> class SynchronizedQueue
{
    std::queue<T> q; std::mutex m; std::condition_variable cv;
    
    public:
    
    T pop ()
    {
        std::unique_lock<std::mutex> lock (m);

        while (q.empty()) cv.wait(lock);

		T t = q.top (); q.pop (); return t;
    }
    
    void push (const T & t)
    {
        std::lock_guard<std::mutex> lock (m);
        
        q.push (t);
        
        cv.notify_one();
    }
};
