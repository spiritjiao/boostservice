#include "MainReactor.h"
#include<iostream>

#include"../util/Logclub.h"
#include"spdlog/spdlog.h"

MainReactor::MainReactor(io_context &ioContext, ip::tcp::endpoint &endpoint):
ioContext_(ioContext),
acceptor_(ioContext,endpoint)
{}

void MainReactor::accept() {
    std::shared_ptr<ip::tcp::socket> socketptr(new ip::tcp::socket(this->subreactor_[index_]->get_io_context()));

    acceptor_.async_accept(*socketptr,[this,socketptr](const boost::system::error_code& errorCode){
      accept_handler(socketptr,errorCode);
    });
}


void MainReactor::accept_handler(std::shared_ptr<ip::tcp::socket> socketptr,std::error_code errorCode) {
    if(errorCode)
    {
        spdlog::error("socket连接失败!{}",errorCode.message());
        socketptr->close();
    }else
    {
        LOG_TYPE::localhost_access->info("{}-{}",socketptr->remote_endpoint().address().to_string(),socketptr->remote_endpoint().port());
        LOG_TYPE::localhost_access->flush();
        spdlog::debug("socket连接成功:ip{} port{}",socketptr->remote_endpoint().address().to_string(),socketptr->remote_endpoint().port());
        subreactor_[index_]->add_socket(socketptr);
        index_++;
        index_%=index_%sub_reactor_size_;
    }
    this->accept();
}

//初始化功能 注册事件
void MainReactor::start() {
    this->sub_reactor_size_=std::thread::hardware_concurrency()*2-2;
    for(int i=0;i<this->sub_reactor_size_;i++)
    {
     this->subreactor_.emplace_back(std::make_unique<SubReactor>());
    }
    spdlog::debug("SubReactor create success!");
   this->accept();
}

void MainReactor::stop() {
    this->acceptor_.close();
    this->ioContext_.stop();
}



