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

//服务端绑定生成endpoint
int server_end_point()
{
    unsigned short port_num = 3333;
    asio::ip::address ip_address = asio::ip::address_v6::any();
    asio::ip::tcp::endpoint ep(ip_address, port_num);
    return 0;
}

//客户端通信socket
int create_tcp_socket()
{
    asio::io_context ioc;
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    asio::ip::tcp::socket sock(ioc);
    boost::system::error_code ec;
    sock.open(protocol, ec);
    if (ec.value() != 0)
    {
        std::cout << "Failed to open the socket!Error coe = " << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }
    return 0;
}

//服务端accept
int create_acceptor_socket()
{
    // //这是老版本写法
    // asio::io_context ios;
    // asio::ip::tcp::acceptor acceptor(ios);
    // asio::ip::tcp protocol = asio::ip::tcp::v4();
    // boost::system::error_code ec;
    // acceptor.open(protocol, ec);
    // if (ec.value() != 0)
    // {
    //     std::cout << "Failed to open the acceptor socket! Error code = " << ec.value() << ".Message: " << ec.message();
    //     return ec.value();
    // }
    //这是新版本写法
    asio::io_context ios;
    asio::ip::tcp::acceptor a(ios, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 3333));
    return 0;
}

//服务端绑定
int bind_acceptor_socket()
{
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;
    asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
    boost::system::error_code ec;
    acceptor.bind(ep, ec);
    if (ec.value() != 0)
    {
        std::cout << "Failed to bind the acceptor socket. Error code = " << ec.value() << ".Message: " << ec.message();
        return ec.value();
    }
    return 0;
}

//客户端连接
int connect_to_end()
{
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_context ios;
        asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
    } catch (system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message :" << e.what();
        return e.code().value();
    }
}

//dns解析连接
int dns_connect_to_end()
{
    std::string host = "llfc.club";
    std::string port_num = "3333";
    asio::io_context ios;
    asio::ip::tcp::resolver::query resolver_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);
    asio::ip::tcp::resolver resolver(ios);
    try
    {
        asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        asio::ip::tcp::socket sock(ios);
        asio::connect(sock, it);
    } catch (system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ".Message : " << e.what();
        return e.code().value();
    }
}

//服务端接收连接
int accept_new_connection()
{
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_context ios;
    try
    {
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);
    } catch (system::system_error &e)
    {
        std::cout << "Error occured!Error code = " << e.code() << ".Message" << e.what();
    }
    return 0;
}