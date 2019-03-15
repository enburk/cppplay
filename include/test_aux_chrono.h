#pragma once

#include "test_aux.h"
#include <chrono>
#include <random>
#include <thread>

struct Time
{
    decltype (std::chrono::high_resolution_clock::now ())
    stamp  =  std::chrono::high_resolution_clock::now ();

    friend auto operator - (Time t1, Time t0)
        { return t1.stamp - t0.stamp; }
};

inline std::ostream & operator << (std::ostream & stream, std::chrono::high_resolution_clock::duration duration)
{
    using std::chrono:: duration_cast;
    auto seconds      = duration_cast<std::chrono::seconds     >(duration); duration -= seconds;
    auto milliseconds = duration_cast<std::chrono::milliseconds>(duration); duration -= milliseconds;
    auto microseconds = duration_cast<std::chrono::microseconds>(duration); duration -= microseconds;
    auto nanoseconds  = duration_cast<std::chrono::nanoseconds >(duration);

    stream.fill ('0');
    stream           << seconds     .count () << "."
    << std::setw (3) << milliseconds.count () << "'"
    << std::setw (3) << microseconds.count () << "'"
    << std::setw (3) << nanoseconds .count (); 

    return stream;
}

inline int randint (int l, int u)
{
    thread_local std::random_device seed;
    thread_local std::mt19937 generator (seed ());
    return std::uniform_int_distribution<int> (l, u) (generator);
}
// thread_local specifier implies the static specifier
// https://stackoverflow.com/questions/185624/static-variables-in-an-inlined-function
// A static local variable in an extern inline function always refers to the same object.
// 7.1.2/4 - C++98/C++14 (n3797)

TEST_ON
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
