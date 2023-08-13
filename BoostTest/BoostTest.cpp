#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>
using namespace std;

//线程池类
class ThreadPool
{
public:
    ThreadPool(size_t threads);//构造函数，传入线程数
    ~ThreadPool();             //析构
    //可变参数模板
    //入队任务（传入参数和函数的参数）
    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> _workers;       // 工作线程组
    std::queue<std::function<void()>> _tasks;//任务队列

    //异步
    std::mutex _queueMutex;            //队列互斥锁
    std::condition_variable _condition;//条件变量
    bool _stop;                        // 线程池停止标志
};

// 设置线程任务，启动一些工作线程
ThreadPool::ThreadPool(size_t threads)
{
    //添加线程到工作线程组
    for (size_t i = 0; i < threads; ++i)
    {
        // 线程内不断的从任务队列取任务执行
        // 1. 从任务队列中获取任务（需要保护临界区）
        // 2. 执行任务
        _workers.emplace_back([this] {
            for (;;)
            {
                std::function<void()> task;

                {
                    // 拿锁(独占所有权式)
                    std::unique_lock<std::mutex> lock(this->_queueMutex);

                    // 等待唤醒（等待条件成立），条件是停止或者任务队列中有任务

                    /*执行条件变量等待的时候，已经拿到了锁(即lock已经拿到锁，没有阻塞)
                    这里将会unlock释放锁，其他线程可以继续拿锁，但此处任然阻塞，等待条件成立
                    一旦收到其他线程notify_*唤醒，则再次lock，然后进行条件判断
                    当[return this->stop || !this->tasks.empty()]的结果为false将阻塞
                    条件为true时候解除阻塞。此时lock依然为锁住状态*/
                    this->_condition.wait(lock, [this] { return this->_stop || !this->_tasks.empty(); });

                    // 如果线程池停止或者任务队列为空，结束返回
                    if (this->_stop && this->_tasks.empty())
                        return;
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                }

                task();
            }
        });
    }
}

//添加一个新的工作任务到线程池
template<typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    // 将任务函数和其参数绑定，构建一个packaged_task
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    //获取任务的future
    std::future<return_type> res = task->get_future();

    {
        // 独占拿锁
        std::unique_lock<std::mutex> lock(_queueMutex);
        //不允许入队到已经停止的线程池
        if (_stop) throw std::runtime_error("enqueue on stopped ThreadPool");

        // 将任务添加到任务队列
        _tasks.emplace([task]() { (*task)(); });
    }

    //发送通知，唤醒某一个工作线程执行任务
    _condition.notify_all();
    return res;
}

ThreadPool::~ThreadPool()
{
    {
        //拿锁
        std::unique_lock<std::mutex> lock(_queueMutex);
        _stop = true;//停止标志置true
    }

    // 通知所有工作线程，唤醒后因为stop为true了，所以都会结束
    _condition.notify_all();
    // 等待所有工作线程结束
    for (std::thread &worker: _workers)
    {
        worker.join();
    }
}

#include <chrono>
#include <iostream>
#include <vector>

int main()
{
    ThreadPool pool(4);//创建线程池，指定线程数量4
    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i)
    {
        results.emplace_back(pool.enqueue(
                [i] {
                    std::cout << "hello " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << "world " << i << std::endl;
                    return i * i;
                }));
    }

    // 遍历存储在 results 中的每个任务的返回结果
    for (auto &&result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}