#include <thread>

void do_something(int& i)
{
    ++i;
}

struct func
{
    int& i;

    func(int& i_):i(i_){} //引用

    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_something(i);// 1 潜在访问隐患：空引用
        }
    }
};

//函数已经返回，线程依旧访问局部变量
void oops()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();  // 2 不等待线程结束 （可以换成join())
}                        // 3 新线程可能还在运行

int main()
{
    oops();
}
