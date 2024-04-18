#include <iostream>
#include "mprpcchannel.h"
#include "user.pb.h"
#include "mprpcapplication.h"//框架头文件
int main(int argc, char **argv) {
    //整个程序启动以后，想使用mprpc框架来享用rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);
    //演示调用远程发布的rpc方法login
    //为了构建stub，必须给出一个rpcchannel，之后调用都是通过channel
    //定义一个代理对象stub对象，传入一个rpcchannel，rpcchannel的核心就是继承了rpcchannel，并重写了callmethod方法
    //callmethod这里涉及基类指针调用派生类对象&&基类的同名覆盖方法
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    //rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    /*
    rpc请求的数组组装，数据的序列化
    发送rpc请求 wait
    接收rpc响应
    响应的反序列化
    */
    //rpc方法的响应
    fixbug::LoginResponse responese;
    //发起rpc方法的调用 这是同步的rpc调用过程，底层是MprpcChannel::callmethod方法的调用，它将发起数据序列化、网络发送、等待远程rpc响应方法执行结果
    //通过代理对象stub，调用rpc方法
    stub.Login(nullptr, &request, &responese, nullptr);//所有通过桩类调用的rpc方法，实际上都是调用的底层rpcchannel的callmethod方法，RpcChannel->RpcChannel::callMethod，集中来做所有rpc方法的参数序列化和网络发送

    //一次rpc调用完成，读调用的结果
    if (responese.result().errcode() == 0) {
        //值=0表示没错误
        std::cout << "rpc login response: " << responese.succuss() << std::endl;
    } else {
        std::cout << "rpc login response erroe: " << responese.result().errmsg() << std::endl;
    } 




    //演示调用远程发布的rpc方法register
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("666");
    fixbug::RrgisterResponse rsp;
    //以同步的方式，发起rpc调用请求，等待返回结果
    stub.Register(nullptr, &req, &rsp, nullptr);

    //一次rpc调用完成，读调用的结果
    if (rsp.result().errcode() == 0) {
        //值=0表示没错误
        std::cout << "rpc register response: " << rsp.succuss() << std::endl;
    } else {
        std::cout << "rpc register response erroe: " << rsp.result().errmsg() << std::endl;
    } 



    return 0;
}
//rpc调用方对于rpc方法的请求进行序列化和网络发送，以（阻塞？）同步的方式等待rpc返回的结果，然后再进行数据的反序列化，把响应结果上报给应用