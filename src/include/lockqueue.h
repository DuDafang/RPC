#pragma once
#include <queue>
#include <thread>
#include <mutex>//线程安全的互斥锁 pthread_mutex_t
#include <condition_variable>//条件变量 pthread_condition_t
//异步写日志的日志队列
template<typename T>
class lockQueue
{
public:
    void Push(const T &data);
    T& Pop();
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};