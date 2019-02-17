#pragma once
#include "aux_aux.h"
#include <future>
#include <thread>
#include <mutex>
#if (_MSC_VER >= 1915)
#include <experimental/generator>
#include <experimental/resumable>
#include <experimental/coroutine>
#include <xmmintrin.h>
#endif

namespace sync
{
    #if (_MSC_VER >= 1915)
    #include "cpp_sync_coroutines.h"
    #endif

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
}
