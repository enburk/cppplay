#include <mutex>
#include <thread>

void sleepsort (std::vector<int> & vector, int microseconds)
{
    int i = 0; std::vector<std::thread> threads; std::mutex mutex; auto procedure = [&](int value)
    {
        std::this_thread::sleep_for (std::chrono::microseconds (value * microseconds));
            
        std::lock_guard<std::mutex> lock (mutex);
            
        vector [i++] = value;
    };

    for (int value : vector) threads.emplace_back (std::thread (procedure, value));

    for (auto & thread : threads) thread.join ();
}

TEST_OFF
{
    std::vector<int> v0 = random (10, 20);

    for (int us = 0; us <= 5000; us += 500)
    {
        std::vector<int> v = v0; sleepsort (v, us);

        for (int i : v) cout << i << " ";

        bool correct = std::is_sorted (v.begin (), v.end ());
        
        cout << "*" << us << (correct ? " correct" : " wrong") << endl;
    }
};

// Possible output:
// 
// 20 13 17 11 21 18 25 22 28 13 24 11 15 25 12 15 26 18 17 10 *0 wrong
// 11 13 12 11 10 13 15 15 18 17 20 18 21 22 17 25 24 25 26 28 *500 wrong
// 11 10 11 12 13 13 15 15 17 18 17 20 18 21 22 25 24 25 26 28 *1000 wrong
// 11 13 11 10 13 12 15 17 15 18 20 17 21 18 22 25 24 25 28 26 *1500 wrong
// 11 11 10 13 12 13 17 15 15 18 17 20 18 21 22 24 25 25 26 28 *2000 wrong
// 11 11 10 13 12 13 15 17 15 18 20 17 18 21 22 24 25 25 26 28 *2500 wrong
// 11 10 11 13 12 13 15 15 17 18 17 18 20 21 22 24 25 25 26 28 *3000 wrong
// 11 10 11 13 12 13 15 15 17 18 17 18 20 21 22 24 25 25 26 28 *3500 wrong
// 11 10 11 13 12 13 15 15 17 18 17 18 20 21 22 24 25 25 26 28 *4000 wrong
// 10 11 11 12 13 13 15 15 17 17 18 18 20 21 22 24 25 25 26 28 *4500 correct
// 10 11 11 12 13 13 15 15 17 17 18 18 20 21 22 24 25 25 26 28 *5000 correct
