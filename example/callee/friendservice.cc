#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid) {
        std::cout << "do GetFriendsList service! userid: " << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao ");
        vec.push_back("liu");
        vec.push_back("wang");
        return vec;
    }

    //重写基类方法
    void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done) 
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendList = GetFriendsList(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string &name : friendList) {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)//框架使用
{   
    //调用框架的初始化操作
    MprpcApplication::Init(argc, argv);//初始化时，传入这两个参数是因为启动的时候要读配置文件

    //procider是一个rpc网络服务对象，把FriendService对象发布到rpc节点上，负责数据的序列化、反序列化、网络数据收发
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    //启动一个rpc服务发布节点，run之后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}