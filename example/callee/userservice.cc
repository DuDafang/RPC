#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

/*
UserService原来所一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc//使用在rpc服务发布端,
//UserService从UserServiceRpc继承而来，UserServiceRpc从Service继承而来
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login " << std::endl;
        std::cout << " name: " << name << " pwd: " << pwd << std::endl;
        return false;
    }

    bool Register(uint32_t id, std::string name, std::string pwd) {
        std::cout << "doing local service: Register " << std::endl;
        std::cout << " id: " << id << " name: " << name << " pwd:" << pwd << std::endl;
        return true;
    }

    /*重写基类UserServiceRpc的虚函数，下面的这些方法都是框架直接调用的
    caller--Login（LoginRequest）--muduo--callee
    callee--Login(LoginRequest)--交到下面重写的这个Login方法上
    */
    void Login(::google::protobuf::RpcController* controller,
                        const::fixbug::LoginRequest* request,
                        ::fixbug::LoginResponse* response,
                        ::google::protobuf::Closure* done)
                        //指的是在 google::protobuf 命名空间中定义的 RpcController 和 Closure 类
                        //使用 :: 前缀（称为全局命名空间限定符）的原因是确保引用的类型或函数是从全局命名空间开始查找的，
                        //而不是从当前命名空间或其他潜在的冲突命名空间开始。这是一种避免命名冲突并明确指定所需命名空间的方法。

                        //在嵌套命名空间中的使用：如果你的代码位于一个或多个嵌套的命名空间中，
                        //使用全局命名空间限定符可以直接跳出所有本地命名空间，直接引用全局命名空间下的实体。
    {
        //框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        //做本地业务
        bool login_result = Login(name, pwd);

        //把响应写入 包括错误码、错误消息、返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");//("login do error!");
        response->set_succuss(login_result);

        //执行回调操作
        done->Run();

    }
    //继承protobuf生成的类，重写register方法
    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RrgisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_succuss(ret);

        done->Run();//执行done的回调，这个回调，框架上做的事情是把填好的response进行序列化，通过网络发回给client
    }
};

int main(int argc, char **argv)//框架使用
{   
    //调用框架的初始化操作
    MprpcApplication::Init(argc, argv);//初始化时，传入这两个参数是因为启动的时候要读配置文件

    //procider是一个rpc网络服务对象，把UserService对象发布到rpc节点上，负责数据的序列化、反序列化、网络数据收发
    RpcProvider provider;
    provider.NotifyService(new UserService());

    //启动一个rpc服务发布节点，run之后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}