//
// Created by zzz梓
//

#ifndef WEBSERVICE_MAINREACTOR_H
#define WEBSERVICE_MAINREACTOR_H
#include<boost/asio.hpp>
#include"SubReactor.h"
#include<memory>
using namespace  boost::asio;
class MainReactor : public std::enable_shared_from_this<MainReactor> {
public:
MainReactor(io_context &ioContext,ip::tcp::endpoint &endpoint);
void start();
void stop();
private:
    void accept();
    void accept_handler(std::shared_ptr<ip::tcp::socket> socket,std::error_code errorCode);

unsigned int  index_=0;
unsigned int  sub_reactor_size_;
io_context &ioContext_;
ip::tcp::acceptor  acceptor_;
std::vector<std::unique_ptr<SubReactor>> subreactor_;
};

#endif //WEBSERVICE_MAINREACTOR_H
