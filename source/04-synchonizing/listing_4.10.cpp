#include <future>
	
class connection_set  {
};

//使用promise解决单线程多连接问题
void process_connections(connection_set& connections)
{
    while(!done(connections))
    {
        for(connection_iterator
                connection=connections.begin(),end=connections.end();
            connection!=end;
            ++connection)//检查每个连接
        {
            if(connection->has_incoming_data())//检索是否有数据
            {
                data_packet data=connection->incoming();
                std::promise<payload_type>& p=
                    connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if(connection->has_outgoing_data())//检索正在发送已入队的传出数据
            {
                outgoing_packet data=
                    connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(true);//表明传输成功
            }
        }
    }
}
