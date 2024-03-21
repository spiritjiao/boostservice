
#ifndef WEBSERVICE_SUBREACTOR_H
#define WEBSERVICE_SUBREACTOR_H
#include<list>
#include<boost/asio.hpp>
#include"../Service/ServiceClub.h"

//管理每个io_context所执行对象
//添加用户并添加服务
class SubReactor {
public:
    SubReactor();
    ~SubReactor();
    boost::asio::io_context& get_io_context();
    void add_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
private:
    //only read
     void recv(std::shared_ptr<boost::asio::ip::tcp::socket> socketptr);
    //process read
     void handler_recv(const boost::system::error_code& error,std::size_t bytes_transferred,std::vector<char> &&buff);
     void clear_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socketptr);
     //void send();
std::array<char,1024*1024*8> buffer_recv;
std::array<char,1024*1024*8> buffer_send;
boost::asio::io_context ioContext_;
std::shared_ptr<boost::asio::io_context::work> work_;
std::list<std::shared_ptr<boost::asio::ip::tcp::socket> > SocketArray_;
std::unique_ptr<std::thread> thread_;
};

#endif //WEBSERVICE_SUBREACTOR_H
