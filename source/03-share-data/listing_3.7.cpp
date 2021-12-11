#include <mutex>

class hierarchical_mutex
{
public:
    explicit hierarchical_mutex(unsigned level)
    {}
    
    void lock()
    {}
    void unlock()
    {}
};

//使用层次锁来避免死锁
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);

int do_low_level_stuff()
{
    return 42;
}


int low_level_func()//层级最低的函数
{
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_stuff(int some_param)
{}

//high_level_mutex(10000)要比low_level_mutex(5000)更高级。
//所以运行没问题。
void high_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}

void thread_a()
{
    high_level_func();
}

hierarchical_mutex other_mutex(6000);
void do_other_stuff()
{}


void other_stuff()
{
    high_level_func();//这个互斥量的层级值是10000，要比当前层级值6000大很多
    do_other_stuff();
}

//无视规则，因此在运行时会失败。
void thread_b()
{
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}

int main()
{}
