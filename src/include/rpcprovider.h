#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
//框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider
{
public:
    //这里是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();

private:

    //组合EventLoop
    muduo::net::EventLoop m_eveentLoop;
    //新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    //已建立连接用户的读写事件回调，muduo1如果发现有可读写的消息，就会主动使用这个函数
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};