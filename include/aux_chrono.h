#include <chrono>
#include <random>
#include <thread>
using namespace std::chrono_literals;

struct Time
{
    // auto stamp = std::chrono::high_resolution_clock::now ();
    // a non-static data member cannot have a type that contains 'auto'

    decltype (std::chrono::high_resolution_clock::now ())
    stamp  =  std::chrono::high_resolution_clock::now ();

    friend auto operator - (Time t1, Time t0) { return t1.stamp - t0.stamp; }
};

std::ostream & operator << (std::ostream & stream, std::chrono::high_resolution_clock::duration duration)
{
    auto seconds      = std::chrono::duration_cast<std::chrono::seconds     >(duration); duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration); duration -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration); duration -= microseconds;
    auto nanoseconds  = std::chrono::duration_cast<std::chrono::nanoseconds >(duration);

    stream.fill ('0');
    stream           << seconds     .count () << "."
    << std::setw (3) << milliseconds.count () << "'"
    << std::setw (3) << microseconds.count () << "'"
    << std::setw (3) << nanoseconds .count (); 

    return stream;
}

int randint (int l, int u)
{
    thread_local std::random_device seed;
    thread_local std::mt19937 generator (seed ());
    return std::uniform_int_distribution<int> (l, u) (generator);
}

int default_randint (int l, int u)
{
    static Time t;
    static auto seed = (unsigned) t.stamp.time_since_epoch ().count ();
    static std::default_random_engine                 generator (seed);
    return std::uniform_int_distribution<int> (l, u) (generator);
}

TEST_OFF
{
    for (int i=0; i<9; i++)
    {
        auto ms = randint (0, 9);
        Time t0; std::this_thread::sleep_for (std::chrono::milliseconds (ms));
        Time t1; cout <<"sleep for " << ms << " ms takes " << t1-t0 << " sec" << endl;
    }
};

// Possible output:
//
// sleep for 5 ms takes 0.005'667'184 sec
// sleep for 1 ms takes 0.001'792'856 sec
// sleep for 6 ms takes 0.006'372'092 sec
// sleep for 0 ms takes 0.000'003'695 sec
// sleep for 0 ms takes 0.000'002'463 sec
// sleep for 8 ms takes 0.008'963'871 sec
// sleep for 5 ms takes 0.005'483'258 sec
// sleep for 4 ms takes 0.004'027'460 sec
// sleep for 6 ms takes 0.006'332'679 sec
