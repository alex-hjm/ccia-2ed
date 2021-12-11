#include <list>
#include <algorithm>
#include <future>

//快速排序——并行版
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),
        divide_point);
    //当前线程不对小于“中间”值部分的列表进行排序，std::async()会使用另一线程对列表进行排序
    std::future<std::list<T> > new_lower(
        std::async(&parallel_quick_sort<T>,std::move(lower_part)));
    auto new_higher(
        parallel_quick_sort(std::move(input)));//使用递归进行排序
    result.splice(result.end(),new_higher);
    //get()返回一个包含结果的右值引用，这样的结果就是可以移动的
    result.splice(result.begin(),new_lower.get());
    return result;
}
