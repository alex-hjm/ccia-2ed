#include <exception>
#include <memory>

//当栈为空时，pop()函数会抛出一个empty_stack异常
struct empty_stack: std::exception
{
    const char* what() const throw();
};

//线程安全的堆栈类定义
template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack&);
    //赋值操作被删除
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    //简化接口更有利于数据控制，可以保证互斥量将操作完全锁住
    void push(T new_value);
    //使用std::shared_ptr可以避免内存分配管理的问题，并避免多次使用new和delete操作
    std::shared_ptr<T> pop();
    void pop(T& value);
    bool empty() const;
};

int main()
{}
