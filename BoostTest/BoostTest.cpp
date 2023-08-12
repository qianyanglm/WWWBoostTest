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

class ThreadPool
{
public:
    ThreadPool(size_t threads);
    ~ThreadPool();
    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> _workers;// 所有工作线程
    std::queue<std::function<void()>> _tasks;

    std::mutex _queueMutex;
    std::condition_variable _condition;
    bool _stop;// 线程池停止标志
};

// 设置线程任务
ThreadPool::ThreadPool(size_t threads)
{
    for (size_t i = 0; i < threads; ++i)
    {
        // 每个线程需要做的事情很简单
        // 1. 从任务队列中获取任务（需要保护临界区）
        // 2. 执行任务
        _workers.emplace_back([this] {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->_queueMutex);
                    // 等待唤醒，条件是停止或者任务队列中有任务
                    this->_condition.wait(lock, [this] { return this->_stop || !this->_tasks.empty(); });
                    if (this->_stop && this->_tasks.empty()) return;
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                }
                task();
            }
        });
    }
}

template<typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    // 将需要执行的任务函数打包（bind），转换为参数列表为空的函数对象
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(_queueMutex);

        if (_stop) throw std::runtime_error("enqueue on stopped ThreadPool");

        // 利用lambda函数包装线程函数，使其符合function<void()>的形式
        // 并且返回值可以通过future获取
        _tasks.emplace([task]() {
            (*task)();
        });
    }

    _condition.notify_all();
    return res;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _stop = true;
    }
    _condition.notify_all();// 唤醒所有线程，清空任务
    for (std::thread &worker: _workers)
    {
        worker.join();
    }
}

#include <chrono>
#include <iostream>

void PrintNumber(int num)
{
    std::cout << "Thread " << std::this_thread::get_id() << ": " << num << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main()
{
    ThreadPool threadPool(4);// 创建一个拥有4个线程的线程池

    for (int i = 0; i < 10; ++i)
    {
        threadPool.enqueue(PrintNumber, i);// 将PrintNumber任务添加到线程池中
    }

    // 添加更多任务...
    auto future1 = threadPool.enqueue([]() { return 42; });
    auto future2 = threadPool.enqueue([]() { return 100; });

    std::cout << "future1的结果: " << future1.get() << std::endl;
    std::cout << "future2的结果: " << future2.get() << std::endl;

    // ThreadPool的析构函数将确保所有任务完成并且线程被加入

    return 0;
}