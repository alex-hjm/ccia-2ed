#include <mutex>
#include <condition_variable>
#include <queue>

//从代码4.1中提取push()和wait_and_pop()
template<typename T>
class threadsafe_queue
{
private:
    std::mutex mut;//互斥量
    std::queue<T> data_queue;
    std::condition_variable data_cond;//条件变量
public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
};


struct data_chunk{};
data_chunk prepare_data();
bool more_data_to_prepare();
void process(data_chunk);
bool is_last_chunk(data_chunk);

//线程队列中有互斥量和条件变量，所以独立的变量就不需要了
threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
        data_chunk const data=prepare_data();
        data_queue.push(data);//push()不需要外部同步
    }
}

void data_processing_thread()
{
    while(true)
    {
        data_chunk data;
        data_queue.wait_and_pop(data);//兼顾条件变量的等待
        process(data);
        if(is_last_chunk(data))
            break;
    }
}
