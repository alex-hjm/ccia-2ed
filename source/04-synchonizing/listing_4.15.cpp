#include <string>

struct card_inserted
{
    std::string account;

};

//ATM逻辑类的简单实现
class atm
{
    messaging::receiver incoming;
    messaging::sender bank;
    messaging::sender interface_hardware;
    void (atm::*state)();
    std::string account;
    std::string pin;
    void waiting_for_card()
    {
        //发送一条信息到接口，让终端显示“等待卡片”的信息
        interface_hardware.send(display_enter_card());
        incoming.wait()
            .handle<card_inserted>(//等待传入一条消息进行处理
                [&](card_inserted const& msg)
                {
                    account=msg.account;//将用户账号信息缓存到成员变量中
                    pin="";//清除PIN信息
                    interface_hardware.send(display_enter_pin());//发送一条消息到硬件接口，让显示界面提示用户输入PIN
                    state=&atm::getting_pin;//将线程状态改为“获取PIN”
                }
                );
    }
    void getting_pin();
public:
    void run()
    {
        state=&atm::waiting_for_card;//初始化waiting_for_card的状态
        try
        {
            for(;;)//反复执行当前状态的成员函数(无论这个状态时怎么样的)
            {
                (this->*state)();
            }
        }
        catch(messaging::close_queue const&)
        {
        }
    }
};
