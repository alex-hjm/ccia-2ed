#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list;
std::mutex some_mutex;

//使用互斥量保护列表
void add_to_list(int new_value)
{
    //对数据的访问是互斥的
    std::lock_guard<std::mutex> guard(some_mutex);
    //=std::lock_guard guard(some_mutex);
    //c++17 : std::scoped_lock guard(some_mutex);
    some_list.push_back(new_value);
}

//不能看到正在被add_to_list()修改的列表
bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(),some_list.end(),value_to_find)
        != some_list.end();
}

#include <iostream>

int main()
{
    add_to_list(42);
    std::cout<<"contains(1)="<<list_contains(1)<<", contains(42)="<<list_contains(42)<<std::endl;
}
