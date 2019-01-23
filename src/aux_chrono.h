#include <chrono>
#include <random>
#include <thread>

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

int random (int l, int u)
{
    static Time t;
    static auto seed = (unsigned) t.stamp.time_since_epoch ().count ();
    static std::default_random_engine                 generator (seed);
    return std::uniform_int_distribution<int> (l, u) (generator);
}

TEST_ON
{
    for (int i=0; i<9; i++)
    {
        auto ms = random (0, 9);
        Time t0; std::this_thread::sleep_for (std::chrono::milliseconds (ms));
        Time t1; cout <<"sleep for " << ms << " ms takes " << t1-t0 << " sec" << endl;
    }
};
