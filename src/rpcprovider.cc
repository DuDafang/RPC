#include "rpcprovider.h"
#include <string>
#include "mprpcapplication.h"
#include <functional>
//这里是框架提供给外部使用的，可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}

//启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverip").c_str());
    muduo::net::InetAddress address(ip, port);

    //创建TcpServer对象
    muduo::net::TcpServer server(&m_eveentLoop, address, "RpcProvider");
    //绑定连接回调和消息读写回调方法    用muduo库很好的分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));//绑定器,std::placeholders::_1预留一个参数,最多可以预留29个参数
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    //设置muduo库的线程数，muduo库自动分发io线程，工作线程；如果线程数量为1，那么io线程与工作线程就在一个线程里面，
    //如果有多个线程，那么有一个线程专门做io线程，接受新用户的连接，生成相应的clientfd，分发给工作线程，剩余的就是工作线程
    server.setThreadNum(4);//基于reactor的服务器模型，epoll+多线程

    //启动网络服务
    server.start();
    m_eveentLoop.loop();//以阻塞的方式等待远程的连接
}


//新的socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{

}

//已建立连接用户的读写事件回调，muduo1如果发现有可读写的消息，就会主动使用这个函数
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp)
{

}