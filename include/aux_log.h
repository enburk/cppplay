#include <vector>
#include <string>

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

struct element
{
    std::string s;

   ~element         (                   )           { log::put ( "dtor: " + s ); }
    element         (                   ) : s ("_") { log::put ( "ctor: " + s ); }
    element         ( char            c ) : s (1,c) { log::put ( "ctor: " + s ); }
    element         ( const element & e ) : s (e.s) { log::put ( "copy: " + s ); }
    element         (       element&& e ) noexcept
                                          : s (std::forward<std::string>(e.s))
                                                    { log::put ( "move: " + s ); }
    void operator = ( char            c )           { log::put ( "assg: " + s + "=" + c   ); s = c;   }
    void operator = ( const element & e )           { log::put ( "copy: " + s + "=" + e.s ); s = e.s; }
    void operator = (       element&& e ) noexcept  { log::put ( "move: " + s + "=" + e.s ); s = std::forward<std::string>(e.s); }
};

TEST_OFF
{
    {
        element a ='a';
        element b = a ; b ='b';
        element c = element ('c');
        log::print ();
    }
    log::print ();

    oops ( std::vector<element> a; a.emplace_back ('1'); ), "", { "ctor: 1", "-------", "dtor: 1" });
//  oops ( std::vector<element> a; a.   push_back ('1'); ), "", { "ctor: 1", "-------", "dtor: 1" }); // should print error log and exit
    oops ( std::vector<element> a; a.   push_back ('1'); ), "", { "ctor: 1", "move: 1", "dtor: ", "-------", "dtor: 1" });
};

// Output:
// 
// ctor: a
// copy: a
// assg: a=b
// ctor: c
// =====
// dtor: c
// dtor: b
// dtor: a
// =====
