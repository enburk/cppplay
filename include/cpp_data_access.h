class NewCreatable { private: ~NewCreatable() {}; friend void destruct (NewCreatable* p) { delete p; }; };
 
TEST_ON
{
    { auto ptr = new NewCreatable; destruct (ptr); }
 
    return 0;
};