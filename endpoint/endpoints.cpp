#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace boost;

//终端节点的创建
int client_end_point()
{
    std::string raw_ip_address = "127.4.8.1";
    unsigned short port_num = 3333;
    boost::system::error_code ec;
    asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);
    if (ec.value() != 0)
    {
        std::cout << "Failed to parse the IP address. Error code =" << ec.value() << ".Message is" << ec.message();
        return ec.value();
    }
    asio::ip::tcp::endpoint ep(ip_address, port_num);
}

//服务器
int server_end_point()
{
    return 0;
}