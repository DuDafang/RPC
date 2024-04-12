#include <iostream>
#include <string>
#include "user.pb.h"
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
};

int main()
{
    return 0;
}