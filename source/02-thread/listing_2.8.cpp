#include <algorithm>
#include <functional>
#include <thread>
#include <vector>

void do_work(unsigned id)
{
}

//量产线程，等待它们结束
void f()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.push_back(std::thread(do_work, i)); // 产生线程
    }
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join)); // 对每个线程调用 join()

    //for (auto &entry : threads) // 对每个线程调用 join()
    //    entry.join();
}

int main()
{
    f();
}
