namespace task_system_3 // Sean Parent - Better Code: Concurrency [NDC 2017]
{
    // Task Stealing

    using namespace std;

    using lock_t = unique_lock<mutex>;

    class notification_queue {
        deque<function<void()>> _q;
        bool                    _done{false};
        mutex                   _mutex;
        condition_variable      _ready;

      public:
        bool pop(function<void()>& x) {
            lock_t lock{_mutex};
             while (_q.empty() && !_done) _ready.wait(lock);
             if (_q.empty()) return false;
             x = move(_q.front());
            _q.pop_front();
            return true;
        }

        bool try_pop(function<void()>& x) {
            lock_t lock{_mutex, try_to_lock};
            if (!lock || _q.empty()) return false;
            x = move(_q.front());
            _q.pop_front();
            return true;
        }

        template<typename F>
        bool try_push(F&& f) {
            {
                lock_t lock{_mutex, try_to_lock};
                if (!lock) return false;
                _q.emplace_back(forward<F>(f));
            }
            _ready.notify_one();
            return true;
        }
        void done() {
            {
                unique_lock<mutex> lock{_mutex};
                _done = true;
            }
            _ready.notify_all();
        }
    };

    class task_system {
        const unsigned              _count{thread::hardware_concurrency()};
        vector<thread>              _threads;
        vector<notification_queue>  _q{_count};
        atomic<unsigned>            _index{0};

        void run(unsigned i) {
            while (true) {
                function<void()> f;

                for (unsigned n = 0; n != _count; ++n) {
                    if (_q[(i + n) % _count].try_pop(f)) break;
                }
                if (!f && !_q[i].pop(f)) break;
                
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
            for (auto& e : _q) e.done();
            for (auto& e : _threads) e.join();
        }

        template <typename F>
        void async_(F&& f) {
            auto i = _index++;

            const int K = 100; // ???

            for (unsigned n = 0; n != _count * K; ++n) {
                if (_q[(i + n) % _count].try_push(forward<F>(f))) return;
            }
            _q[i % _count].push(forward<F>(f));
        }
    };
}
