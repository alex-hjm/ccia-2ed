#include <experimental/future>

//使用并发技术扩展规范中的特性，实现与`std::async`等价的功能
template<typename Func>
std::experimental::future<decltype(std::declval<Func>()())>
spawn_async(Func&& func){
    std::experimental::promise<
        decltype(std::declval<Func>()())> p;
    auto res=p.get_future();
    std::thread t(
        [p=std::move(p),f=std::decay_t<Func>(func)]()
            mutable{
            try{//将函数的结果存储在future中
                p.set_value_at_thread_exit(f());
            } catch(...){//或捕获函数抛出的异常，将异常存到future中
                p.set_exception_at_thread_exit(std::current_exception());
            }
    });
    t.detach();
    return res;
}
