#include <thread>

void do_something(int& i)
{
    ++i;
}

struct func
{
    int& i;

    func(int& i_):i(i_){}

    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_something(i);
        }
    }
};

void do_something_in_current_thread()
{}

//等待线程完成
void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        do_something_in_current_thread();
    }
    catch(...)
    {
        t.join();//当执行过程中抛出异常，程序会执行到此处
        throw;
    }
    t.join();//当函数正常退出后，会执行到此处。
}

int main()
{
    f();
}
