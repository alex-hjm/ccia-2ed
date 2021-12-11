#include <future>
#include <string>

struct X
{
    void foo(int, std::string const &);
    std::string bar(std::string const &);
};

//使用 std::async 向函数传递参数
X x;
// 调用p->foo(42, "hello")，p是指向x的指针
auto f1 = std::async(&X::foo, &x, 42, "hello");
// 调用tmpx.bar("goodbye")， tmpx是x的拷贝副本
auto f2 = std::async(&X::bar, x, "goodbye");

struct Y
{
    double operator()(double);
};
Y y;
// 调用tmpy(3.141)，tmpy通过Y的移动构造函数得到
auto f3 = std::async(Y(), 3.141);
auto f4 = std::async(std::ref(y), 2.718);// 调用y(2.718)
X baz(X &);
auto f6 = std::async(baz, std::ref(x));// 调用baz(x)
class move_only
{
  public:
    move_only();
    move_only(move_only &&);
    move_only(move_only const &) = delete;
    move_only &operator=(move_only &&);
    move_only &operator=(move_only const &) = delete;
    void operator()();
};
auto f5 = std::async(move_only());// 调用tmp()，tmp是通过std::move(move_only())构造得到
