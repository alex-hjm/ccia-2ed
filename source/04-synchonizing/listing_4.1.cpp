#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

bool more_data_to_prepare()
{
    return false;
}

struct data_chunk
{};

data_chunk prepare_data()
{
    return data_chunk();
}

void process(data_chunk&)
{}

bool is_last_chunk(data_chunk&)
{
    return true;
}

//使用std::condition_variable处理数据等待
std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
        data_chunk const data=prepare_data();
        std::lock_guard<std::mutex> lk(mut);//锁定队列
        data_queue.push(data);//将准备好的数据压入队列
        data_cond.notify_one();//对等待的线程(如果有等待线程)进行通知
    }
}

void data_processing_thread()
{
    while(true)
    {
        std::unique_lock<std::mutex> lk(mut);//首先对互斥量上锁
        //当条件满足时返回，条件不满足将解锁互斥量，并且将线程置于阻塞或等待状态
        data_cond.wait(lk,[]{return !data_queue.empty();});//Lambda表达式作为等待的条件
        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main()
{
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}
