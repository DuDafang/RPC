#include "rpcprovider.h"

#include "mprpcapplication.h"

#include "rpcheader.pb.h"
/*
service_name对应一个service描述
                    service描述对应一个服务指针service*记录服务对象
                    method_name对应method方法对象
*/
//这里是框架提供给外部用户使用的，用户可以发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{   
    ServiceInfo service_info;
    //获取了服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    //获取服务的名字
    std::string service_name = pserviceDesc->name();
    //获取服务对象service的方法的数量
    int methodCnt = pserviceDesc->method_count();
    std::cout << "service_name:" << service_name << std::endl;
    for (int i = 0; i < methodCnt; i++) {
        //获取了服务对象指定下标的服务方法的描述（抽象描述），框架不能写具体的服务类的名字
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        std::cout << "method_name:" << method_name << std::endl;
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});//服务的名字，服务的具体描述
}

//启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    //创建TcpServer对象
    muduo::net::TcpServer server(&m_eveentLoop, address, "RpcProvider");
    //绑定连接回调和消息读写回调方法    用muduo库很好的分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));//绑定器,std::placeholders::_1预留一个参数,最多可以预留29个参数
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    //设置muduo库的线程数，muduo库自动分发io线程，工作线程；如果线程数量为1，那么io线程与工作线程就在一个线程里面，
    //如果有多个线程，那么有一个线程专门做io线程，接受新用户的连接，生成相应的clientfd，分发给工作线程，剩余的就是工作线程
    server.setThreadNum(4);//基于reactor的服务器模型，epoll+多线程

    std::cout << "RpcProvider starrt service at ip: " << ip << std::endl;

    //启动网络服务
    server.start();

    m_eveentLoop.loop();//以阻塞的方式等待远程的连接
}


//新的socket连接回调
//rpc请求相当于短连接
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{
    if (!conn->connected()) {
        //和rpc client的连接断开了，此时要关闭文件描述符
        conn->shutdown();
    }
}

/*
在框架内部，rpcprovider和rpcconsumer协商好之间通信用的protubuf数据类型
携带过来的数据应该包含：service_name method_name args

                        service_name method_name args_size

定义proto的message类型，进行数据头的序列化和反序列化

header_size(4个字节) + header_str() + args_str()
记录参数的长度防止粘包（？）
*/


//已建立连接用户的读写事件回调，muduo1如果发现有可读写的消息，就会主动使用这个函数
//如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp)
{
    //网络上接收的远程rpc调用请求的字符流，该字符流应该包含rpc方法的名字、参数、
    std::string recv_buf = buffer->retrieveAllAsString();
}