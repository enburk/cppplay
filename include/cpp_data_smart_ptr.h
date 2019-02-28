// https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared

This function may be used as an alternative to std::shared_ptr<T>(new T(args...)). The trade-offs are:

std::shared_ptr<T>(new T(args...)) performs at least two allocations (one for the object T and one for the control block of the shared pointer), while std::make_shared<T> typically performs only one allocation (the standard recommends, but does not require this, all known implementations do this)
If any std::weak_ptr references the control block created by std::make_shared after the lifetime of all shared owners ended, the memory occupied by T persists until all weak owners get destroyed as well, which may be undesirable if sizeof(T) is large.
std::shared_ptr<T>(new T(args...)) may call a non-public constructor of T if executed in context where it is accessible, while std::make_shared requires public access to the selected constructor.
Unlike the std::shared_ptr constructors, std::make_shared does not allow a custom deleter.
std::make_shared uses ::new, so if any special behavior has been set up using a class-specific operator new, it will differ from std::shared_ptr<T>(new T(args...)).
std::shared_ptr supports array types (as of C++17), but std::make_shared does not. This functionality is supported by boost::make_shared
(until C++20)
code such as f(std::shared_ptr<int>(new int(42)), g()) can cause a memory leak if g gets called after new int(42) and throws an exception, while f(std::make_shared<int>(42), g()) is safe, since two function calls are never interleaved.
(until C++17)
A constructor enables shared_from_this with a pointer ptr of type U* means that it determines if U has an unambiguous and accessible (since C++17) base class that is a specialization of std::enable_shared_from_this, and if so, the constructor evaluates the statement:

if (ptr != nullptr && ptr->weak_this.expired())
  ptr->weak_this = std::shared_ptr<std::remove_cv_t<U>>(*this,
                                  const_cast<std::remove_cv_t<U>*>(ptr));
Where weak_this is the hidden mutable std::weak_ptr member of std::shared_from_this. The assignment to the weak_this member is not atomic and conflicts with any potentially concurrent access to the same object. This ensures that future calls to shared_from_this() would share ownership with the shared_ptr created by this raw pointer constructor.

The test ptr->weak_this.expired() in the exposition code above makes sure that weak_this is not reassigned if it already indicates an owner. This test is required as of C++17.