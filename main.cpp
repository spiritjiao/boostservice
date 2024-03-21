#include<boost/asio.hpp>
#include"reactor/MainReactor.h"
#include <boost/asio/signal_set.hpp>
#include "spdlog/spdlog.h"


//接受外部调用
void signal_handler(const boost::system::error_code& err, int &signal)
{
      if(signal==SIGINT) {
          spdlog::info("stop by ctrl c");
      }
}


int main(){
    spdlog::set_level(spdlog::level::info);

    try {
        io_context main_ioContext;
        ip::tcp::endpoint Main_endpoint(ip::tcp::v4(), 8888);

        auto reactor=std::make_shared<MainReactor>(main_ioContext, Main_endpoint);
        //二次初始化
        reactor->start();

        //监控外部信号
        boost::asio::signal_set signals(main_ioContext, SIGINT);
        //检测信号
        signals.async_wait([&](const boost::system::error_code &err, int signal) {
            signal_handler(err, signal);
            reactor->stop();
        });

        spdlog::info("MainReactor服务器开始事件循环...");
        main_ioContext.run();
    }
    catch (boost::system::system_error & systemError) //runtime_error
    {
       spdlog::error("boost runtime_error:{}",systemError.what());
    }catch(std::error_code &errorCode)
    {
        spdlog::error("exception:{}",errorCode.message());
    }
    catch(std::exception &exception)      //exception error
    {
        spdlog::error("exception:{}",exception.what());
    }

}
