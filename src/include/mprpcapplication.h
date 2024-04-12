#pragma once

//mprpc框架的基础类,负责框架的一些初始化操作
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);//一种传统的方式来接收命令行参数
    //argc表示命令行参数的数量，包括程序名称本身，所以至少为1
    //argv是一个字符指针数组，每个元素指向一个以null结尾的C字符串，表示每个命令行参数
    static MprpcApplication& GetInstance();//方法写在对应的cc文件
private:
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};