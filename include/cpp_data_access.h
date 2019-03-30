class NewCreatable { private: ~NewCreatable() {}; friend void destruct (NewCreatable* p) { delete p; }; };
 
TEST_ON
{
    { auto ptr = new NewCreatable; destruct (ptr); }
 
    return 0;
};

/*
namespace Log // anonymous namespace exposes using declarations
{
    using std::vector;
    using std::string;

    struct log
    {
        // using std::vector; // doesn't work inside class/struct
        // using std::string; // doesn't work inside class/struct

        static inline vector<string> Log;

        static void print () { for (auto s : Log) cout << s << endl; cout << "=====" << endl; Log.clear (); }

        static void put   (const string & s) { Log.push_back (s); }

        static void check (const string & title, const vector<string> & true_log )
        {
            if ( Log    != true_log ){ cout << "===== " << title << " =====" << endl; print (); exit (1); } else Log.clear ();
        }
    };
}

string s;
*/