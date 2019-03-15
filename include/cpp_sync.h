#pragma once
#include "aux_aux.h"
#include <csignal>
#include <future>
#include <thread>
#include <mutex>
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

