#pragma once
#include "lockqueue.h"
enum LogLevel
{
    INFO, //普通的日志信息
    ERROR, //错误信息
};
//Mprpc框架提供的日志系统
class Logger
{
public:
    void SetLogLevel(LogLevel level);//设置日志的级别
    //写日志
    void Log(std::string msg);
private:
    int m_loglevel;//记录日志级别
    lockQueue<std::string> m_lckQue;//日志缓存队列

    Logger();
    //为了设置成单例模式，把拷贝构造都delete
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
}; 