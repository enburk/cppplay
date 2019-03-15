// CppCon 2015: Andrei Alexandrescu “Declarative Control Flow"

// https://github.com/yuri-kilochek/boost.scope_guard


class UncaughtExceptionCounter
{

public:

UncaughtExceptionCounter() : exceptionCount_(std::uncaught_exceptions()) {}

bool isNewUncaughtException() noexcept
{
    return std::uncaught_exceptions() > exceptionCount_;
}

private:
int exceptionCount_;
};

template <typename FunctionType>
class ScopeGuardForNewException
{
public:

explicit ScopeGuardForNewException(const FunctionType & fun) : 
function_(fun){}

explicit ScopeGuardForNewException(FunctionType && fun) : 
function_(std::move(fun)) {}

~ScopeGuardForNewException() 
{
    if (exceptionCounter_.isNewUncaughtException())
        function_();
}

private:
FunctionType function_;
UncaughtExceptionCounter exceptionCounter_;
};

enum class ScopeGuardOnFail {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type> operator 
+ (ScopeGuardOnFail, FunctionType && fun)
{
    return ScopeGuardForNewException<typename 
    std::decay<FunctionType>::type> 
    (std::forward<FunctionType>(fun));
}

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

#define SCOPE_FAIL auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) = 
::detail::ScopeGuardOnFail() + [&]()



void copyFileTransact(const bf::path &from, const bf::path &to)
{
    bf::path temp = std::string("temp.deleteme");
    SCOPE_FAIL{ ::remove(temp.string().c_str()); };
    bf::copy_file(from, temp);
    bf::rename(temp, to);
}