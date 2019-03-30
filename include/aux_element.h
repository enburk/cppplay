struct element
{
    std::string s;

   ~element         (                  ) noexcept(false)           { log::put ( "dtor: " + s ); }
    element         (                  ) noexcept(false) : s ("_") { log::put ( "ctor: " + s ); }
    element         (char             c) noexcept(false) : s (1,c) { log::put ( "ctor: " + s ); }
    element         (const element  & e) noexcept(false) : s (e.s) { log::put ( "copy: " + s ); }
    element         (      element && e) noexcept(false) : s (std::forward<std::string>(e.s))
                                                                   { log::put ( "move: " + s ); }
    void operator = (char             c) noexcept(false)           { log::put ( "assg: " + s + "=" + c   ); s = c;   }
    void operator = (const element  & e) noexcept(false)           { log::put ( "copy: " + s + "=" + e.s ); s = e.s; }
    void operator = (      element && e) noexcept(false)           { log::put ( "move: " + s + "=" + e.s ); 
                                                          s = std::forward<std::string>(e.s); }
};

struct only_copyable : element
{
   ~only_copyable              (                         ) = default;
    only_copyable              (                         ) = default;
    only_copyable              (const element        & e ) : element (e) {}
    only_copyable              (      element       && e ) : element (e) {}
    only_copyable              (const only_copyable  & e ){};// = default;
    only_copyable              (      only_copyable && e ) = delete;
    only_copyable & operator = (const only_copyable  & e ) = default;
    only_copyable & operator = (      only_copyable && e ) = delete;
};

struct only_movable : element
{
    only_movable               (const element        & e ) : element (e) {}
    only_movable               (      element       && e ) : element (e) {}
    only_movable               (const only_movable   & e ) = delete;
    only_movable               (      only_movable  && e ) = default;
    only_movable  & operator = (const only_movable   & e ) = delete;
    only_movable  & operator = (      only_movable  && e ) = default;
};

struct only_makable : element
{
    only_makable               (const element        & e ) : element (e) {}
    only_makable               (      element       && e ) : element (e) {}
    only_makable               (const only_makable   & e ) = delete;
    only_makable               (      only_makable  && e ) = delete;
    only_makable  & operator = (const only_makable   & e ) = delete;
    only_makable  & operator = (      only_makable  && e ) = delete;
};

using neither_copyable_nor_movable = only_makable;

TEST_ON
{
    {
        element a ='a';
        element d = a ;
        element b = a ; b = 'b';
        element c = element ('c');
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

    using std::vector;

    oops ( vector<element> v; v.emplace_back ('1'); ), { "ctor: 1", "-------", "dtor: 1" });
//  oops ( vector<element> v; v.   push_back ('1'); ), { "ctor: 1", "-------", "dtor: 1" }); // print error log and exit
    oops ( vector<element> v; v.   push_back ('1'); ), { "ctor: 1", "move: 1", "dtor: ", "-------", "dtor: 1" });
};

