#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"

/*
UserService原来所一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc//使用在rpc服务发布端
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
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
        code->set_errmsg("");
        response->set_succuss(login_result);

        //执行回调操作
        done->Run();

    }


};

int main()
{
    return 0;
}