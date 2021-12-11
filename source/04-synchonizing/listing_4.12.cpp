#include <list>
#include <algorithm>

//快速排序——串行版
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    //将输入的首个元素(中间值)放入结果列表中
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();//使用了引用，避免过多的拷贝
    //std::partition会将区间[first,last)中的元素重新排列，满足判断条件的元素
    //会被放在区间的前段，不满足的元素会被放在区间的后段。
    //返回值返回区间后段元素的首元素的迭代器
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    //将input列表小于divided_point的值移动到新列表lower_part中,其他数继续留在input列表中
    lower_part.splice(lower_part.end(),input,input.begin(),
        divide_point);
    //指向的值放在“中间”值的前面
    auto new_lower(
        //显式使用 std::move()将列表传递到函数
        sequential_quick_sort(std::move(lower_part)));//递归
    //指向的值放在“中间”值的后面
    auto new_higher(
        sequential_quick_sort(std::move(input)));
    //将result中的结果以正确的顺序进行拼接
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower);
    return result;
}

