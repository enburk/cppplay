namespace task_system_1 // Sean Parent - Better Code: Concurrency [NDC 2017]
{
    using namespace std;

    using lock_t = unique_lock<mutex>;

    class notification_queue {
        deque<function<void()>> _q;
        mutex                   _mutex;
        condition_variable      _ready;

      public:
        void pop(function<void()>& x) {
            lock_t lock{_mutex};
             while (_q.empty()) _ready.wait(lock);
             x = move(_q.front());
            _q.pop_front();
        }

        template<typename F>
        void push(F&& f) {
            {
                lock_t lock{_mutex};
                _q.emplace_back(forward<F>(f));
            }
            _ready.notify_one();
        }
    };

    class task_system {
        const unsigned      _count{thread::hardware_concurrency()};
        vector<thread>      _threads;
        notification_queue  _q;

        void run(unsigned i) {
            while (true) {
                function<void()> f;
                _q.pop(f);
                f();
            }
        }

      public:
        task_system() {
            for (unsigned n = 0; n != _count; ++n) {
                _threads.emplace_back([&, n]{ run(n); });
            }
        }

        ~task_system() {
            for (auto& e : _threads) e.join();
        }

        template <typename F>
        void async_(F&& f) {
            _q.push(forward<F>(f));
        }
    };
}