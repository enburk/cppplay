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
