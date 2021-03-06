#pragma once
#include "aux_aux.h"

namespace fast
{
    #include "cpp_fast_lambda.h"
    #include "cpp_fast_math.h"
}

// CPUS HAVE A HIERARCHICAL CACHE SYSTEM by Jeff Dean
// 
// One cycle on a 3 GHz processor                 1   ns
// L1 cache reference                             0.5 ns
// Branch mispredict                              5   ns
// L2 cache reference                             7   ns                14x L1 cache
// Mutex lock/unlock                             25   ns
// Main memory reference                        100   ns      20x L2,   200x L1
// Compress 1K bytes with Snappy              3.000   ns
// Send 1K bytes over 1 Gbps network         10.000   ns      0.01 ms
// Read 4K randomly from SSD*               150.000   ns      0.15 ms
// Read 1 MB sequentially from memory       250.000   ns      0.25 ms
// Round trip within same datacenter        500.000   ns      0.5  ms
// Read 1 MB sequentially from SSD*       1.000.000   ns      1    ms   4x memory
// Disk seek                             10.000.000   ns     10    ms   20x datacenter RT
// Read 1 MB sequentially from disk      20.000.000   ns     20    ms   80x memory, 20x SSD
// Send packet CA->Netherlands->CA      150.000.000   ns    150    ms


// let's test it!

// recursion

// fedor - weak strong exchange

// x86 false sharing