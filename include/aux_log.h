struct log
{
    static inline std::vector<std::string> Log;

    static void print () { for (auto s : Log) cout << s << endl; cout << "=====" << endl; Log.clear (); }

    static void put   (const std::string & s) { Log.push_back (s); }

    static void check (const std::string & title, const std::string & report, const std::vector<std::string> & true_log )
    {
        if ( report != ""       ){ cout << "===== " << title << " =====" << endl << report; exit (1); }
        if ( Log    != true_log ){ cout << "===== " << title << " =====" << endl; print (); exit (1); } else Log.clear ();
    }
};

#define oops(...) { __VA_ARGS__; log::put ("-------"); }; log::check (#__VA_ARGS__

