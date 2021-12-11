void atm::getting_pin()
{
    incoming.wait()
        .handle<digit_pressed>(//输入数字
            [&](digit_pressed const& msg)
            {
                unsigned const pin_length=4;
                pin+=msg.digit;
                if(pin.length()==pin_length)//最终输入
                {
                    bank.send(verify_pin(account,pin,incoming));
                    state=&atm::verifying_pin;
                }
            }
            )
        .handle<clear_last_pressed>(//清除最后一位数字
            [&](clear_last_pressed const& msg)
            {
                if(!pin.empty())
                {
                    pin.resize(pin.length()-1);
                }
            }
            )
        .handle<cancel_pressed>(//取消输入
            [&](cancel_pressed const& msg)
            {
                state=&atm::done_processing;
            }
            );
}
