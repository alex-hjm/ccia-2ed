#include <thread>

//使用RAII等待线程完成
class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_):
        t(t_)
    {}
    ~thread_guard()
    {
        if(t.joinable()) //判断线程是否可汇入
        {
            t.join(); //2 调用join()进行汇入
        }
    }
    //为了不让编译器自动生成，这可能会弄丢已汇入的线程
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

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


void f()
{
    int some_local_state;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
        
    do_something_in_current_thread();
}   //线程执行到此处时，局部对象就要被逆序销毁了
    //这时线程在析构函数中被加入到原始线程中

int main()
{
    f();
}
