#include <iostream>
#include <string>
/*
UserService原来所一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class UserService
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
    }
};
