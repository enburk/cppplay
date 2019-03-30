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

// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c

template <class T> constexpr std::string_view type_name () // Howard Hinnant et al.
{
    using namespace std;
    
    #ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);

    #elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
    #if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
    #else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
    #endif

    #elif defined(_MSC_VER)
    const  string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
    #endif
}

TEST_OFF
{
    const int ci = 0;
    const int & cri = ci;

    cout << type_name<decltype(ci)>() << endl;
    cout << type_name<decltype(cri)>() << endl;

    cout << type_name<decltype(detail::log::mutex)>() << endl;
    cout << type_name<decltype(detail::log::print)>() << endl;
    cout << type_name<decltype(detail::log::Log  )>() << endl;
};