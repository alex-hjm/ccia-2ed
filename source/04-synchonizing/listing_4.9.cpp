#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

//使用 std::packaged_task 执行一个图形界面线程
std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

bool gui_shutdown_message_received();
void get_and_process_gui_message();

void gui_thread()
{
    while(!gui_shutdown_message_received())
    {
        get_and_process_gui_message();//进行轮询界面消息处理
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if(tasks.empty())//当队列中没有任务，循环将继续
                continue;
            task=std::move(tasks.front());//除非能在队列中提取出一个任务
            tasks.pop_front();
        }
        task();//释放队列上的锁，并且执行任务
    }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)//将任务传入队列
{
    std::packaged_task<void()> task(f);//提供一个打包好的任务
    std::future<void> res=task.get_future();//调用get_future()成员函数获取future对象
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;//并且在任务推入列表之前，future将返回调用函数。
}
