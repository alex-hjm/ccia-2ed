#include <memory>

//线程安全队列的接口
template<typename T>
class threadsafe_queue
{
public:
    threadsafe_queue();
    threadsafe_queue(const threadsafe_queue&);
    threadsafe_queue& operator=(const threadsafe_queue&) = delete;// 不允许简单的赋值

    void push(T new_value);

    //尝试从队列中弹出数据，即使没有值可检索，也会直接返回
    bool try_pop(T& value);
    std::shared_ptr<T> try_pop();//当没有值可检索时，这个函数返回NULL

    //将会等待有值可检索的时候才返回
    void wait_and_pop(T& value);
    std::shared_ptr<T> wait_and_pop();

    bool empty() const;
};

int main()
{}
