// CppCon 2018: Arthur O'Dwyer “Return Value Optimization: Harder Than It Looks”

struct From1 {
    element e;
    From1 (element e) : e (e) {}
};
struct From2 {
    element e;
    From2 (element const & e) : e (e) {}
    From2 (element &&      e) : e (e) {}
};
struct To {
    element e;
    operator element () const & { return e; }
    operator element () &&      { return e; }
};

TEST_OFF
{
    auto one   = []() -> From1   { element e{'1'}; return e; };
    auto two   = []() -> From2   { element e{'2'}; return e; };
    auto three = []() -> element { To      t{'3'}; return t; };

    oops ( From1   f = one   () ), {"ctor: 1", "copy: 1", "copy: 1", "dtor: 1", "dtor: 1", "-------", "dtor: 1"}); // MSVC
//  oops ( From1   f = one   () ), {"ctor: 1", "copy: 1", "dtor: 1", "-------", "dtor: 1"});
//  oops ( From1   f = one   () ), {"ctor: 1", "move: 1", "dtor: " , "-------", "dtor: 1"}); // proposal P1155, GCC already
    oops ( From2   f = two   () ), {"ctor: 2", "copy: 2", "dtor: 2", "-------", "dtor: 2"});
//  oops ( From2   f = two   () ), {"ctor: 2", "move: 2", "dtor: " , "-------", "dtor: 2"}); // proposal P1155
    oops ( element e = three () ), {"ctor: 3", "copy: 3", "dtor: 3", "-------", "dtor: 3"});
//  oops ( element e = three () ), {"ctor: 3", "move: 3", "dtor: " , "-------", "dtor: 3"}); // proposal P1155

    auto five  = [](element e) -> element     { return e; };
    auto six   = [](element e) -> void        { throw  e; };
    auto seven = []() -> void { element e{'7'}; throw  e; };

    oops ( try { element e = five ('5'); } catch (...){} ), {"ctor: 5", "move: 5", "dtor: ", "dtor: 5", "-------"});
    oops ( try { six ('6');              } catch (...){} ), {"ctor: 6", "move: 6", "dtor: ", "dtor: 6", "-------"});
    oops ( try { seven ();               } catch (...){} ), {"ctor: 7", "move: 7", "dtor: ", "dtor: 7", "-------"});

    auto eight = [](element && e) -> element { return e; };
    auto nine  = [](element &  e) -> element { element&& r = std::move(e); return r; };

    oops ( element e{'8'}; element ee = eight (std::move(e)); ), {"ctor: 8", "copy: 8", "-------", "dtor: 8", "dtor: 8"});
//  oops ( element e{'8'}; element ee = eight (std::move(e)); ), {"ctor: 8", "move: 8", "-------", "dtor: " , "dtor: 8"}); // proposal P0527
    oops ( element e{'9'}; element ee = nine  (e);            ), {"ctor: 9", "copy: 9", "-------", "dtor: 9", "dtor: 9"});
//  oops ( element e{'9'}; element ee = nine  (e);            ), {"ctor: 9", "move: 9", "-------", "dtor: " , "dtor: 9"}); // proposal P0527
};
