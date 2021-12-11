#include <memory>
#include <mutex>

struct some_resource
{
    void do_something()
    {
    }
};

//使用延迟初始化(线程安全)的过程
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;
void foo()
{
    std::unique_lock<std::mutex> lk(resource_mutex); // 所有线程在此序列化
    if (!resource_ptr)
    {
        resource_ptr.reset(new some_resource); // 只有初始化过程需要保护
    }
    lk.unlock();
    resource_ptr->do_something();
}

//双重检查锁模式
void undefined_behaviour_with_double_checked_locking()
{
    if (!resource_ptr) // 指针第一次读取数据不需要获取锁（未被锁保护的读取操作）
    {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr) // 再检查一次指针，避免另一线程在第一次检查后再做初始化
        {
            resource_ptr.reset(new some_resource); // （被锁保护的写入操作）
        }
    }
    resource_ptr->do_something(); // 4
}

//使用了 std::call_once
namespace
{
std::once_flag resource_flag; // 1

void init_resource() //初始化通过调用函数完成
{
    resource_ptr.reset(new some_resource);
}

void foo1()
{
    std::call_once(resource_flag, init_resource); // 可以完整的进行一次初始化
    resource_ptr->do_something();
}

} // namespace

int main()
{
    foo();
}
