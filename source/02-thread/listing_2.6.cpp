#include <thread>
#include <utility>

//scoped_thread的用法
class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_):
        t(std::move(t_))
    {
        if(!t.joinable())
            throw std::logic_error("No thread");//当线程不可汇入时抛出异常
    }
    ~scoped_thread()
    {
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
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
    //新线程会直接传递到scoped_thread中，而非创建一个独立变量
    scoped_thread t(std::thread(func(some_local_state)));
        
    do_something_in_current_thread();
} //scoped_thread对象就会销毁，然后在析构函数中完成汇入

int main()
{
    f();
}
