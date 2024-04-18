#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.h"//框架头文件

int main(int argc, char **argv) {
    //整个程序启动以后，想使用mprpc框架来享用rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);
    //演示调用远程发布的rpc方法login
    //为了构建stub，必须给出一个rpcchannel，之后调用都是通过channel
    //定义一个代理对象stub对象，传入一个rpcchannel，rpcchannel的核心就是继承了rpcchannel，并重写了callmethod方法
    //callmethod这里涉及基类指针调用派生类对象&&基类的同名覆盖方法
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    //rpc方法的请求参数
    fixbug::GetFriendsListRequest request;
    request.set_userid(10000);
    /*
    rpc请求的数组组装，数据的序列化
    发送rpc请求 wait
    接收rpc响应
    响应的反序列化
    */
    //rpc方法的响应
    fixbug::GetFriendsListResponse responese;
    //发起rpc方法的调用 这是同步的rpc调用过程，底层是MprpcChannel::callmethod方法的调用，它将发起数据序列化、网络发送、等待远程rpc响应方法执行结果
    //通过代理对象stub，调用rpc方法

    MprpcController controller;
    stub.GetFriendsList(&controller, &request, &responese, nullptr);//所有通过桩类调用的rpc方法，实际上都是调用的底层rpcchannel的callmethod方法，RpcChannel->RpcChannel::callMethod，集中来做所有rpc方法的参数序列化和网络发送

    //一次rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    } else {
        if (responese.result().errcode() == 0) {
            //值=0表示没错误
            std::cout << "rpc GetFriendsList response  success! " << std::endl;
            int size = responese.friends_size();
            for (int i = 0; i < size; ++i) {
                std::cout << "index: " << (i + 1) << "name: " << responese.friends(i) << std::endl;
            }

        } else {
            std::cout << "rpc GetFriendsList response erroe: " << responese.result().errmsg() << std::endl;
        }
    }

   
    return 0;

}