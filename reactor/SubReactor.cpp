//
// Created by zzz梓
//

#include "SubReactor.h"
#include<boost/asio/thread_pool.hpp>
#include"spdlog/spdlog.h"
SubReactor::SubReactor() {
        this->thread_=std::make_unique<std::thread>([this](){
            this->work_=std::make_shared<boost::asio::io_service::work>(this->ioContext_);
            this->ioContext_.run();
        });
}


boost::asio::io_context &SubReactor::get_io_context() {
    return this->ioContext_;
}


void SubReactor::add_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
 this->ioContext_.post([this,socket](){
    this->SocketArray_.emplace_back(socket);
    //添加进入管理socket
    //开始接受消息
    this->recv(socket);
    socket->send(boost::asio::buffer("HTTP/1.1 200 OK\\r\\n"));

  });

 ioContext_.poll_one();
}

SubReactor::~SubReactor() {
  ioContext_.stop();
  this->thread_->join();
}

void SubReactor::recv(std::shared_ptr<boost::asio::ip::tcp::socket> socketptr) {
    socketptr->async_read_some(boost::asio::buffer(this->buffer_recv),
                             [this,socketptr](const boost::system::error_code &error, std::size_t bytes_transferred){
                                spdlog::debug("ip:{} port:{} recv:{}",socketptr->remote_endpoint().address().to_string(),socketptr->remote_endpoint().port(),bytes_transferred);
                                 if(bytes_transferred==0)
                                 {
                                     socketptr->close();
                                     this->clear_socket(socketptr);
                                     return;
                                 }
                                 this->buffer_recv[bytes_transferred]='\0';
                                 spdlog::debug("\n{}",buffer_recv.data());
                                 recv(socketptr);
                             });
}

void SubReactor::handler_recv(const boost::system::error_code &error, std::size_t bytes_transferred,
                              std::vector<char> &&buff) {

}

void SubReactor::clear_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socketptr) {
   for(auto liter=this->SocketArray_.begin();liter!=SocketArray_.end();liter++)
   {
       if(socketptr==*liter) {
           this->SocketArray_.erase(liter);
           break;
       }
   }
}






