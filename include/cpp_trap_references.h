int f1(int &x, int c) {
   c  = c - 1;
   if (c == 0) return 1;
   x = x + 1;
   int r = f1(x, c) * x;
   //cout << x << endl;
   return r;
} 
int f2(int &x, int c) {
   c  = c - 1;
   if (c == 0) return 1;
   x = x + 1;
   int r = f2(x, c) * x;
   cout << x << endl;
   return r;
} 
TEST_OFF
{
    { int p = 5; cout << f1(p,p) << endl; }
    { int p = 5; cout << f2(p,p) << endl; }
    // VisualStudio.15.Release/15.9.6+28307.344 x64:
    // 6561 // 9*9*9*9
    // 9
    // 8
    // 7
    // 9
    // 4536 // 9*8*7*9
};