#include <mutex>

namespace detail // anonymous namespaces expose using declarations
// namespace log // error C2757: 'log': a symbol with this name already exists
//               // and therefore this name cannot be used as a namespace name
{
    using std::vector;
    using std::string;

    struct log
    {
        // using std::vector; // doesn't work inside class/struct
        // using std::string; // doesn't work inside class/struct

        static inline std::recursive_mutex mutex;

        static inline vector<string> Log;

        static void put (const string & s) {
            std::lock_guard lock(mutex);
            Log.push_back(s);
        }

        static void print () {
            std::lock_guard lock(mutex);
            for (auto s : Log) cout << s << endl;
            cout << "=====" << endl;
            Log.clear();
        }

        static void check (const string & title, const vector<string> & true_log) {
            std::lock_guard lock(mutex);
            if ( Log != true_log ) {
                cout << "===== " << title << " =====" << endl;
                print();
                exit(1);
            }
            Log.clear();
        }
    };
}

using detail::log;

#define oops(...) { __VA_ARGS__; log::put ("-------"); }; log::check (#__VA_ARGS__

TEST_OFF
{
    log::put ("test 1");
    log::print ();
    log::put ("test 2");
    log::print ();

    oops ( log::put ("test 1"); ), {"test 1", "-------"});
//  oops ( log::put ("test 2"); ), {"test 1", "-------"});
    oops ( log::put ("test 2"); ), {"test 2", "-------"});
};

