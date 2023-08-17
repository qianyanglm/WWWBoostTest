#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class Thread
{
public:
    using ThreadFunction = std::function<void()>;

    Thread(ThreadFunction func1, int id): func(func1), thread_id(id) {}

    void Start()
    {
        std::cout << "thread " << thread_id << " start working" << std::endl;
        thread = std::make_shared<std::thread>(func);
    }

    void Stop()
    {
        if (thread->joinable())
        {
            thread->join();
        }
    }

    ~Thread()
    {
        std::cout << "thread " << thread_id << " end working" << std::endl;
    }

private:
    std::shared_ptr<std::thread> thread;
    int thread_id;
    ThreadFunction func;
};

class ThreadPool
{
public:
    ThreadPool(): threads_nums(0){};

    ~ThreadPool()
    {
        for (int i = 0; i < threads_nums; ++i)
        {
            thread_pool[i]->Stop();
            delete thread_pool[i];
        }
    }

    void Start(int threads_num)
    {
        threads_nums = threads_num;
        for (int i = 0; i < threads_num; ++i)
        {
            thread_pool.push_back(new Thread(std::bind(&ThreadPool::RunInThread, this, i), i));
        }

        for (int i = 0; i < threads_num; ++i)
        {
            thread_pool[i]->Start();
        }
    }

private:
    std::vector<Thread *> thread_pool;
    int threads_nums;

    void RunInThread(int id)
    {
        std::cout << "RunInThread id = " << id << std::endl;
    }
};

int main()
{
    ThreadPool pool;
    pool.Start(8);

    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    return 0;
}