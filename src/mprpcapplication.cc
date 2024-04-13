#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;//类的静态成员变量在类外初始化

void showArgsHelp()
{
    std::cout<<"format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2) {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) 
        {
        case 'i'://有配置文件了
            config_file = optarg;//配置文件的名字
            break;
        case '?'://出现了不希望出现的参数
            //std::cout << "invalid args!" << std::endl;
            showArgsHelp();
            exit(EXIT_FAILURE);
        case ':'://有 -i但是没带参数
            //std::cout << "need <configfile>!" << std::endl;
            showArgsHelp();
            exit(EXIT_FAILURE);         
        default:
            break;
        }
    }
    //开始加载配置文件 rpcserver_ip    rpcserver_port  zookeeper_ip    zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());//LoadConfigFile需要的是char *所以不能直接传config_file

    std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}