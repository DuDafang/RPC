#include "test.pb.h"
#include <iostream>
#include<string>
using namespace fixbug;

int main()
{
    // LoginRequest req;
    // req.set_name("zhang");
    // req.set_pwd("123");

    // std::string send_str;
    // if (req.SerializeToString(&send_str)) {
    //     std::cout << send_str.c_str() << std::endl;
    
    // }
    // //从send_str反序列化一个login请求对象
    // LoginRequest reqB;
    // if (reqB.ParseFromString(send_str)) {
    //     std::cout << reqB.name() << std::endl;
    //     std::cout << reqB.pwd() << std::endl;
    // }



    // LoginResponse rsp;
    // ResultCode * rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录失败");

    // std::cout << rsp.success() << std::endl;
    // std::cout << rsp.result().errmsg() << std::endl;



    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();//如果消息中成员变量本身，又是一种对象，那用mutable_result()，然后会返回相应成员对象的指针，通过指针修改成员对象里面的数据
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();
    user1->set_name("lisi");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;
    return 0;
}
//g++ main.cc test.pb.cc -lprotobuf -lpthread
//./a.out
//注意文件路径