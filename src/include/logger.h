#pragma once
#include "lockqueue.h"
#include <string>
enum LogLevel
{
    INFO, //普通的日志信息
    ERROR, //错误信息
};
//Mprpc框架提供的日志系统
class Logger
{
public:
    //获取日志的单例
    static Logger& GetInstance();
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
//定义宏，以可变参的方式提供更加方便的日志写入方法
#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat,  ##__VA_ARGS__); \
        logger.Log(c); \
    } while (0);

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    } while (0);