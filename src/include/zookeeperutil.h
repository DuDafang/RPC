#pragma once

#include <semaphore.h>//信号量
#include <zookeeper/zookeeper.h>
#include <string>
//封装的zk客户端类
class ZkClient//因为提供的是c的代码，这里做了一个类，把能用到的api封装
{
public:
    ZkClient();
    ~ZkClient();
    //zkclient启动连接zkserver
    void Start();
    //在zkserver上根据指定的path创建znode节点
    void Create(const char *path, const char *data, int datalen, int state=0);
    std::string GetData(const char *path);
private:
    //zk的客户端句柄
    zhandle_t *m_zhandle;
};