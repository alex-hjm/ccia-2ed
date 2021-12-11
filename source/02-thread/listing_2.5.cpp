#include <thread>

void some_function()
{}

void some_other_function(int)
{}

//函数返回std::thread对象 线程的所有权可以在函数外进行转移
std::thread f()
{
    void some_function();
    return std::thread(some_function);
}
std::thread g()
{
    void some_other_function(int);
    std::thread t(some_other_function,42);
    return t;
}

int main()
{
    std::thread t1=f();
    t1.join();
    std::thread t2=g();
    t2.join();
}
