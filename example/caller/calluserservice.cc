#include <iostream>
#include "mprpcapplication.h"//框架头文件
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char **argv) {
    //整个程序启动以后，想使用mprpc框架来享用rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);
    //演示调用远程发布的rpc方法login
    //为了构建stub，必须给出一个rpcchannel，之后调用都是通过channel
    fixbug::UserService_Stub stub(new MprpcChannel());
    //stub.Login();//所有通过桩类调用的rpc方法，实际上都是调用的底层rpcchannel的callmethod方法，RpcChannel->RpcChannel::callMethod，集中来做所有rpc方法的参数序列化和网络发送


    return 0;
}