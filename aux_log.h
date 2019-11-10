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

        struct checker {
            string title;
            checker (string && title) : title (title) {}
            void operator == (const std::vector<string> & true_log) {
                check (title, true_log);
            }
        };
    };
}

using detail::log;

#define oops(...) { __VA_ARGS__; log::put ("-------"); }; log::checker (#__VA_ARGS__) == std::vector<std::string>

TEST_OFF
{
    log::put ("test 1");
    log::print ();
    log::put ("test 2");
    log::print ();

    oops ( log::put ("test 1"); ) {"test 1", "-------"};
//  oops ( log::put ("test 2"); ) {"test 1", "-------"}; // should print error log and exit
    oops ( log::put ("test 2"); ) {"test 2", "-------"};
};

namespace console  // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
{
    const auto black   = "\033[30m"; namespace background { const auto black   = "\033[40m"; }
    const auto red     = "\033[31m"; namespace background { const auto red     = "\033[41m"; }
    const auto green   = "\033[32m"; namespace background { const auto green   = "\033[42m"; }
    const auto yellow  = "\033[33m"; namespace background { const auto yellow  = "\033[43m"; }
    const auto blue    = "\033[34m"; namespace background { const auto blue    = "\033[44m"; }
    const auto magenta = "\033[35m"; namespace background { const auto magenta = "\033[45m"; }
    const auto cyan    = "\033[36m"; namespace background { const auto cyan    = "\033[46m"; }
    const auto white   = "\033[37m"; namespace background { const auto white   = "\033[47m"; }

    namespace bright    { const auto on = "\033[1m"; const auto off = "\033[21m"; }
    namespace underline { const auto on = "\033[4m"; const auto off = "\033[24m"; }

    const auto reset = "\033[0m";
}

TEST_OFF
{
    using namespace console;

    cout << red << "red" << " " << white << "white" << endl;
    cout << bright::on << yellow << "bright yellow" << endl;
    cout << underline::on << magenta << "underline magenta" << endl;
    cout << green << "green" << " " << reset << "regular" << endl;
    cout << blue << background::white << "blue on white" << endl;
    cout << reset << endl;
};
