#pragma once
#include "mprpcconfig.h"
//mprpc框架的基础类,负责框架的一些初始化操作
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);//一种传统的方式来接收命令行参数
    //argc表示命令行参数的数量，包括程序名称本身，所以至少为1
    //argv是一个字符指针数组，每个元素指向一个以null结尾的C字符串，表示每个命令行参数
    static MprpcApplication& GetInstance();//方法写在对应的cc文件

    static MprpcConfig* ;
private:
    static MprpcConfig m_config;

    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};
/*MprpcApplication类使用了单例设计模式
理由如下：构造函数被声明为私有
进制复制和移动构造，复制构造函数和移动构造函数都被删除(=delete)，阻止了通过复制或移动来创建额外的实例
静态获取实例，GetInstance() 方法是静态的，返回对唯一实例的引用；该方法内部使用静态局部变量来存储唯一实例，确保只创建一次，并在首次调用时进行初始化
*/