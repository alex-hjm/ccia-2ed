#include <mutex>

class some_big_object {};

void swap(some_big_object& lhs, some_big_object& rhs) {}

class X {
 private:
  some_big_object some_detail;
  mutable std::mutex m;

 public:
  X(some_big_object const& sd) : some_detail(sd) {}

  friend void swap(X& lhs, X& rhs) {
    if (&lhs == &rhs) return;
    std::lock(lhs.m, rhs.m);  //锁住两个互斥量
    //提供std::adopt_lock参数除了表示std::lock_guard可获取锁之外，
    //还将锁交由std::lock_guard管理
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);

    //=std::scoped_lock<std::mutex,std::mutex> guard(lhs.m,rhs.m);
    //=std::scoped_lock guard(lhs.m,rhs.m);(C++17)
    swap(lhs.some_detail, rhs.some_detail);
  }
};

int main() {}
