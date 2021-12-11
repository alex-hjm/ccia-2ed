#include <climits>
#include <mutex>
#include <stdexcept>


//简单的层级互斥量实现
class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value; // 当前层级值
    unsigned long previous_hierarchy_value; // 前一线程的层级值
    // 使用了thread_local的值来代表当前线程的层级值
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation() // 检查是否违反层级结构
    {
        if (this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value()
    {
         // 先存储当前线程的层级值（用于解锁时恢复）
        previous_hierarchy_value = this_thread_hierarchy_value;
        // 再把其设为锁的层级值
        this_thread_hierarchy_value = hierarchy_value;
    }

  public:
    explicit hierarchical_mutex(unsigned long value) : hierarchy_value(value), previous_hierarchy_value(0)
    {
    }

    void lock()
    {
        check_for_hierarchy_violation();// 要求线程层级值大于锁的层级值
        internal_mutex.lock();    //内部互斥锁已锁住
        update_hierarchy_value(); //一旦成功锁住，就可以更新层级值了
    }

    //调用unlock()对层级值进行保存
    void unlock()
    {
        //为了避免无序解锁造成层次混乱，不是解锁最近上锁的那个互斥量，就需要抛出异常
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated"); 
        // 恢复前一线程的层级值
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    //当互斥量上的锁被一个线程持有，它将返回false
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};

//初始化为最大值以使构造锁时能通过检查
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

// 设定值来表示层级
hierarchical_mutex high(10000);
hierarchical_mutex mid(6000);
hierarchical_mutex low(5000);

void lf() {  // 最低层函数
  std::scoped_lock l(low);
}

void hf() {
  std::scoped_lock l(high);
  lf();  // 可以调用低层函数
}

void mf() {
  std::scoped_lock l(mid);
  hf();  // 中层调用了高层函数，违反了层次结构
}

int main()
{
    hf();  // OK
    mf();  // 错误 
}
