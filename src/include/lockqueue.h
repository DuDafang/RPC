#pragma once
#include <queue>
#include <thread>
#include <mutex>//线程安全的互斥锁 pthread_mutex_t
#include <condition_variable>//条件变量 pthread_condition_t
//异步写日志的日志队列
template<typename T>//模板写的无法分源文件
class lockQueue
{
public:
    //多个worker线程都会写日志queue
    void Push(const T &data)//Push是框架中的工作线程去调用，POP是专门的写日志线程调的
    {
        std::lock_guard<std::mutex> lock(m_mutex);//类似智能指针的lock_guard，可以自动加锁释放锁        获取到了锁
    
        m_queue.push(data);
        //出括号后锁自动释放


        m_condvariable.notify_one();//notify_one 与 all，如果只有一个线程在队列取东西、写日志文件，所以用one，如果有多个线程都队列取东西、写日志文件，
        //那么业务线程在往队列中写东西，应该用all，把所有写日志线程都唤醒，去竞争一把锁，谁竞争到锁谁就去写日志文件
    }

    //一个线程读日志queue，写日志文件
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);//unique_lock是条件变量需要的
        while (m_queue.empty()) {//用while防止线程的虚假唤醒
            //日志队列为空，线程进入wait状态，入队和出队是同一把锁，进入wait后把锁释放掉
            m_condvariable.wait(lock);
        }
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};