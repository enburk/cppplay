struct element
{
    std::string s;

   ~element         () noexcept(false)             { log::put ( "dtor: " + s ); }
    element         () noexcept(false)   : s ("_") { log::put ( "ctor: " + s ); }
    element         (char             c) : s (1,c) { log::put ( "ctor: " + s ); }
    element         (const element  & e) : s (e.s) { log::put ( "copy: " + s ); }
    element         (      element && e) : s (std::move(e.s))
                                                   { log::put ( "move: " + s ); }
    void operator = (char             c)           { log::put ( "assg: " + s + "=" + c   ); s = c;   }
    void operator = (const element  & e)           { log::put ( "copy: " + s + "=" + e.s ); s = e.s; }
    void operator = (      element && e)           { log::put ( "move: " + s + "=" + e.s ); 
                                           s = std::move(e.s); }
};

struct only_copyable : element
{
   ~only_copyable              (                         ) = default;
    only_copyable              (                         ) = default;
    only_copyable              (const element        & e ) : element (e) {}
    only_copyable              (      element       && e ) : element (std::move(e)) {}
    only_copyable              (const only_copyable  & e ) = default;
    only_copyable              (      only_copyable && e ) = delete;
    only_copyable & operator = (const only_copyable  & e ) = default;
    only_copyable & operator = (      only_copyable && e ) = delete;
};

struct only_movable : element
{
   ~only_movable               (                         ) = default;
    only_movable               (                         ) = default;
    only_movable               (const element        & e ) : element (e) {}
    only_movable               (      element       && e ) : element (std::move(e)) {}
    only_movable               (const only_movable   & e ) = delete;
    only_movable               (      only_movable  && e ) = default;
    only_movable  & operator = (const only_movable   & e ) = delete;
    only_movable  & operator = (      only_movable  && e ) = default;
};

struct only_makable : element
{
   ~only_makable               (                         ) = default;
    only_makable               (                         ) = default;
    only_makable               (const element        & e ) : element (e) {}
    only_makable               (      element       && e ) : element (std::move(e)) {}
    only_makable               (const only_makable   & e ) = delete;
    only_makable               (      only_makable  && e ) = delete;
    only_makable  & operator = (const only_makable   & e ) = delete;
    only_makable  & operator = (      only_makable  && e ) = delete;
};

using neither_copyable_nor_movable = only_makable;

TEST_OFF
{
    {
        element a ='a';
        element b = a ; b = 'b';
        element c = element{'c'};
        log::print ();
    }
    log::print ();

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

    using std::vector;

    oops ( vector<element> v; v.emplace_back ('1'); ) { "ctor: 1", "-------", "dtor: 1" };
//  oops ( vector<element> v; v.   push_back ('1'); ) { "ctor: 1", "-------", "dtor: 1" }; // print error log and exit
    oops ( vector<element> v; v.   push_back ('1'); ) { "ctor: 1", "move: 1", "dtor: ", "-------", "dtor: 1" };
};

TEST_OFF
{
    {
        only_copyable c {'c'};
        only_movable  m {'m'};
        only_copyable cc = c;
    //  only_copyable cc = std::move(c); // error C2280: attempting to reference a deleted function
    //  only_movable  mm = m;            // error C2280: attempting to reference a deleted function
        only_movable  mm = std::move(m);
        log::print ();
    }
    log::print ();

    // Output:
    // 
    // ctor: c
    // move: c
    // dtor:
    // ctor: m
    // move: m
    // dtor:
    // copy: c
    // move: m
    // =====
    // dtor: m
    // dtor: c
    // dtor:
    // dtor: c
    // =====
};

