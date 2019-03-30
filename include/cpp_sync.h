#pragma once
#include "aux_aux.h"
#include <csignal>
#include <future>
#include <queue>
//#include <experimental/atomic>
//#include <experimental/future>
#if (_MSC_VER >= 1915)
#include <experimental/generator>
#include <experimental/resumable>
#include <experimental/coroutine>
#include <xmmintrin.h>
#endif

namespace sync
{
    #include "cpp_sync_exceptions.h"
    #include "cpp_sync_memory_order.h"
    #include "cpp_sync_queue.h"
    #include "cpp_sync_rcu.h"
    #include "cpp_sync_spinlock.h"
    #include "cpp_sync_task_parent_1.h"
    #include "cpp_sync_task_parent_2.h"
    #include "cpp_sync_task_parent_3.h"
    #include "cpp_sync_thread_local.h"
    #include "cpp_sync_signal.h"

    #if (_MSC_VER >= 1915)
    #include "cpp_sync_coroutines.h"
    #endif
}

/*
// https://en.cppreference.com/w/cpp/thread/yield
// "busy sleep" while suggesting that other threads run 
// for a small amount of time
void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}
 
int main()
{
    auto start = std::chrono::high_resolution_clock::now();
 
    little_sleep(std::chrono::microseconds(100));
 
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "waited for "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " microseconds\n";
}*/