// Concurrency, Parallelism and Coroutines by Anthony Williams [CppCon  2017]

future<remote_data>
async_get_data(key_type key);
future<data> retrieve_data(
key_type key){
auto rem_data=
co_await async_get_data(key);
co_return process(rem_data);
}


auto f=std::for_each(
parallel_as_coroutine,
v.begin(),v.end(),do_stuff);
co_await f;



stdexp::future<int> fail(){
return stdexp::make_exceptional_future(
std::runtime_error("failed"));
}
void next(stdexp::future<int> f){
f.get();
}
void foo(){
auto f=fail().then(next);
f.get();
}


template<typename F>
auto unwrapped(F f){
return [f=std::move(f)](auto fut){
return f(fut.get());
};
}
stdexp::future<int> find_the_answer();
std::string process_result(int);
auto f=find_the_answer();
auto f2=f.then(unwrapped(process_result));



template<typename ... Futures>
stdexp::future<stdexp::when_any_result<
std::tuple<Futures...>>>
when_any(Futures... futures);
template<typename Iterator>
stdexp::future<stdexp::when_any_result<
std::vector<
std::iterator_traits<Iterator>::
value_type>>>
when_any(Iterator begin,Iterator end);


future<result> parallel_func(data_type data){
auto divided_data=
co_await parallel_divide(data);
auto res1=parallel_func(divided_data.first);
auto res2=parallel_func(divided_data.second);
auto final_result=
co_await parallel_combine(
co_await res1,co_await res2);
co_return final_result;
}