#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

//并行版的 std::accumulate 累加
template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=std::distance(first,last);

    if(!length)//如果输入的范围为空,就会得到init的值
        return init;

    unsigned long const min_per_thread=25;
    //用范围内元素的总数量除以线程(块)中最小任务数，从而确定启动线程的最大数量
    unsigned long const max_threads=
        (length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads=
        std::thread::hardware_concurrency();//返回并发线程的数量

    //计算量的最大值和硬件支持线程数，较小的值为启动线程的数量
    unsigned long const num_threads=
        std::min(hardware_threads!=0?hardware_threads:2,max_threads);

    //每个线程中处理的元素数量，是范围中元素的总量除以线程的个数
    unsigned long const block_size=length/num_threads;

    //存放中间结果
    std::vector<T> results(num_threads);
    //因为在启动之前已经有了一个线程(主线程)，所以启动的线程数必须比num_threads少1。
    std::vector<std::thread>  threads(num_threads-1);

    Iterator block_start=first;
    for(unsigned long i=0;i<(num_threads-1);++i)
    {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);//block_end迭代器指向当前块的末尾
        threads[i]=std::thread( //启动一个新线程为当前块累加结果
            accumulate_block<Iterator,T>(),
            block_start,block_end,std::ref(results[i]));
        block_start=block_end;//当迭代器指向当前块的末尾时，启动下一个块
    }

    //启动所有线程后，该线程会处理最终块的结果
    accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);
    
    //等待创建线程
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));
    //for (auto& entry : threads)
    //      entry.join();  

    //所有结果进行累加
    return std::accumulate(results.begin(),results.end(),init);
}

int main()
{
    std::vector<int> vi;
    for(int i=0;i<1000000;++i)
    {
        vi.push_back(5);
    }
    int sum=parallel_accumulate(vi.begin(),vi.end(),0);
    std::cout<<"sum="<<sum<<std::endl;
}
