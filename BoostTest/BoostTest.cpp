/*
 * 简单的C++11线程池，管理一个任务队列和一个工作线程队列，工作线程不断的从任务队列中取任务，然后执行。如果没有任务就等待鑫新任务的到来，添加新任务的时候先添加到任务队列，然后通知任意（条件变量notify_one）一个线程有新的任务来了
 */
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
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
    //构造函数，传入线程数
    ThreadPool(size_t threads);
    //析构函数
    ~ThreadPool();
    //入队任务（传入函数和函数的参数，可变参数）
    //函数包装模板
    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    //工程线程组
    std::vector<std::thread> workers;
    //任务队列
    std::queue<std::function<void()>> tasks;

    //异步
    //队列互斥锁
    std::mutex queueMutex;
    //条件变量
    std::condition_variable condition;
    // 线程池停止标志
    bool stop;
};

// 构造函数仅启动一些工作线程
inline ThreadPool::ThreadPool(size_t threads): stop(false)
{
    for (size_t i = 0; i < threads; ++i)
    {
        // 每个线程需要做的事情很简单
        // 1. 从任务队列中获取任务（需要保护临界区）
        // 2. 执行任务
        workers.emplace_back(
                [this]
                {
            for(;;)
            {
                std::function<void()> task;

                //{}控制生命周期来控制析构的时间点来更好的利用RAII，task()前就被Unlock了，避免无意义的锁占用
                {
                    //拿锁（独占所有权式）
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    // 等待唤醒，条件是停止或者任务队列中有任务
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    // 执行条件变量等待的时候，已经拿到了锁(即lock已经拿到锁，没有阻塞)
                        // 这里将会unlock释放锁，其他线程可以继续拿锁，但此处任然阻塞，等待条件成立
                        // 一旦收到其他线程notify_*唤醒，则再次lock，然后进行条件判断
                        // 当[return this->stop || !this->tasks.empty()]的结果为false将阻塞
                        // 条件为true时候解除阻塞。此时lock依然为锁住状态

                        // 如果线程池停止或者任务队列为空，结束返回
                    if (this->stop && this->tasks.empty())
                        return;

                    //取得队列首任务
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                //执行任务
                task();
            } });
    }
}

//添加一个新的工作任务到线程池
template<typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    // 将需要执行的任务函数打包（bind），转换为参数列表为空的函数对象
    //将任务函数和其参数绑定，构建一个packaged_task
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    //获取任务的future
    std::future<return_type> res = task->get_future();

    {
        //独占拿锁
        std::unique_lock<std::mutex> lock(queueMutex);

        //不允许入队到已经停止的线程池
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        // 利用lambda函数包装线程函数，使其符合function<void()>的形式
        // 并且返回值可以通过future获取
        //将任务添加到任务队列
        tasks.emplace([task]()
                      { (*task)(); });
    }

    //发送通知，唤醒某一个工作线程取执行任务
    condition.notify_all();
    return res;
}

ThreadPool::~ThreadPool()
{
    {
        //拿锁
        std::unique_lock<std::mutex> lock(queueMutex);
        //停止标志置true
        stop = true;
    }

    // 通知所有工作线程，唤醒后因为stop为true了，所以都会结束
    condition.notify_all();

    //等待所有工作线程结束
    for (std::thread &worker: workers)
    {
        worker.join();
    }
}

int main()
{
    ThreadPool pool(4);// 创建一个拥有4个工作线程的线程池
    //定义一个future容器保存任务结果
    std::vector<std::future<int>> results;

    //循环提交8个任务到线程池
    for (int i = 0; i < 8; ++i)
    {
        // 通过enqueue提交lambda任务, 任务中输出i, 睡眠1秒, 最后返回i* i。
        // enqueue返回该任务的future, emplace_back插入到results保存。
        results.emplace_back(
                pool.enqueue([i]
                             {
                    std::cout << "hello " << i << std::endl;

                    //让当前线程休息1s
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    std::cout << "world " << i << std::endl;
                    return i * i; }));
    }

    // 遍历results获取所有任务结果。
    for (auto &&result: results)
        //通过future的get阻塞等待任务执行结果
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}