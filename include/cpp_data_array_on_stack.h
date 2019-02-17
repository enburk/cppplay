// you can use _alloca() to dynamically allocate variable amounts of memory on the stack
// but: without alloca compiler would free to optimize use of %rbp - less stack usage + extra register

// https://stackoverflow.com/questions/8049657/stack-buffer-based-stl-allocator

// https://chromium.googlesource.com/chromium/chromium/+/master/base/stack_container.h

// boost::static_vector

// http://www.stlsoft.org/doc-1.9/classstlsoft_1_1auto__buffer.html
// http://www.stlsoft.org/doc-1.9/auto__buffer_8hpp-source.html

// https://github.com/charles-salvia/charles/blob/master/stack_allocator.hpp

// http://howardhinnant.github.io/stack_alloc.html


// CppCon 2014: Chandler Carruth "Efficiency with Algorithms, Performance with Data Structures" (on first question)
// C++11: move constructor shouldn't invalidate iterators
// that prevents stack-to-heap vector



// datae: tree, list etc.: store number of traverse if low bits of pointer