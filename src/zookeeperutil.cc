#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

//全局的watcher观察器 zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {//回调的消息类型是和会话相关的消息类型
        if (state == ZOO_CONNECTED_STATE) { //zkclient和zkserver连接成功
            sem_t *sem = (sem_t*)zoo_get_context(zh);//从指定的句柄上获取信号量
            sem_post(sem);//给信号量资源+1
        } 
    }
}
ZkClient::ZkClient() : m_zhandle(nullptr)
{

}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr) {
        zookeeper_close(m_zhandle);//关闭句柄，释放资源，类似于MySQL_Conn
    }
}
//连接zkserver
void ZkClient::Start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    /*
    zookeeper_mt : 多线程版本
    zookeeper的api客户端程序提供了三个线程
    API调用线程
    网络IO线程 pthread_create 底层用poll
    当客户端接收到响应之后，watcher回调线程
    */
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);//zookeeper提供的API
    if (nullptr == m_zhandle) {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    sem_t sem;//信号量
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);//主线程等待
    std::cout << "zookeeper_init success!" << std::endl;
}