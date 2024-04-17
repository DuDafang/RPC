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
        //和rpc client的连接断开了，此时要关闭文件描述符，也就是socket
        conn->shutdown();
    }
}

/*
在框架内部，rpcprovider和rpcconsumer协商好之间通信用的protubuf数据类型
携带过来的数据应该包含：service_name method_name args

                        在数据头里面记录后面参数的长度service_name method_name args_size

定义proto的message类型，进行数据头的序列化和反序列化

header_size(4个字节，表示数据头的长度，也就是除了方法参数之外的数据的长度：方法所属服务的名字，方法的名字) + header_str() + args_str()（在数据头里面记录后面参数的长度）
记录参数的长度防止粘包（？）
上述的4字节的整数数值，是二进制，占用四字节，用的std::string insert和copy方法
需要解决TCP粘包的问题
*/


//已建立连接用户的读写事件回调，muduo1如果发现有可读写的消息，就会主动使用这个函数
//如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp)
{
    //网络上接收的远程rpc调用请求的字符流，该字符流应该包含rpc方法的名字、参数、
    std::string recv_buf = buffer->retrieveAllAsString();
    //从字符流中读取前四个字节的内容
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);//从第0个字节拷贝4个字节
    //根据header_size读取数据头的原始字符流，反序列化数据，得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str)){
        //数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        //数据头反序列化失败
        std::cout << "rpc_header_str:" << rpc_header_str << "parse error!" << std::endl;
        return;
    };
    //获取rpc方法参数的字符流数据
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    //打印调试信息
    std::cout << "==============================" << std::endl;
    std::cout << "header_size" << header_size << std::endl;
    std::cout << "rpc_header_str" << rpc_header_str << std::endl;
    std::cout << "service_name" << service_name << std::endl;
    std::cout << "method_name" << method_name << std::endl;
    std::cout << "args_size" << args_size << std::endl;
    std::cout << "args_str" << args_str << std::endl;
    std::cout << "==============================" << std::endl;

    //获取service对象和method对象
    auto it = m_serviceMap.find(service_name);//这里是为了防止service_name不存在，直接用中括号会有副作用
    if (it == m_serviceMap.end()) {//本节点上这个服务不存在
        std::cout << service_name << "is not exist" << std::endl;
        return;
    }

    //判断方法存不存在
    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        std::cout << service_name << ":" << method_name << "is not exist" << std::endl;
        return;
    }

    google::protobuf::Service *service = it->second.m_service;//获取service对象 也就是对应的new UserService，就是想发布的，从NotifyService发布的方法
    const google::protobuf::MethodDescriptor *method = mit->second;//获取method对象 这个可以对应UserService中的login方法

    //有了服务对象与方法，就开始调用，最终调用到用户上的login方法上,就是userservice里面的

    //生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();//获取了指定服务对象中某个请求的类型和响应的类型，
    //实际上上述代码就是，在抽象的层面给即将调用的userservice的login方法产生了loginrequest对象，框架会把loginrequest对象传过来（传哪？由框架传给本地业务）
    if (!request->ParseFromString(args_str)){//true的话表示解析成功，false表示解析失败
        std::cout << "request parse error, contentL " << args_str << std::endl;
        return;
    };
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    //给下面的method方法的调用，绑定一个closure的回调函数
    google::protobuf::Closure *done = 
    google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>(this, &RpcProvider::SendRpcResponse, conn, response);//生成一个新的回调,扔给CallMethod

    //在框架上根据远端rpc请求，调用当前rpc节点上调用的方法

    //相当于userservice对象在调用login方法 new UserService().Login(controller, request, response, done);要把这个框架抽象
    service->CallMethod(method, nullptr, request, response, done);
}
//closure的回调操作，用于序列化rpc的响应和网络发送
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response) {

}