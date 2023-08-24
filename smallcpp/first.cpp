#include <Windows.h>
#include <future>
#include <iostream>
#include <thread>
using namespace std;
#define _WINDOWSLJQ_

class A
{
public:
    A()
    {
#ifdef _WINDOWSLJQ_
        InitializeCriticalSection(&my_winsec);//初始化临界区
#endif
    }

    virtual ~A()
    {
#ifdef _WINDOWSLJQ_
        DeleteCriticalSection(&my_winsec);//释放临界区
#endif
    }

    //把收到的消息放入队列的线程
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; ++i)
        {
            cout << "inMsgRecvQueue()执行，插入一个元素: " << i << endl;
#ifdef _WINDOWSLJQ_
            EnterCriticalSection(&my_winsec);//进入临界区
            msgRecvQueue.push_back(i);
            LeaveCriticalSection(&my_winsec);//离开临界区
#else
            my_mutex.lock();
            msgRecvQueue.push_back(i);
            my_mutex.unlock();
#endif
        }
    }

    bool outMsgLULProc(int &command)
    {
#ifdef _WINDOWSLJQ_
        EnterCriticalSection(&my_winsec);
        if (!msgRecvQueue.empty())
        {
            int command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            LeaveCriticalSection(&my_winsec);
            return true;
        }
        LeaveCriticalSection(&my_winsec);
#else
        my_mutex.lock();
        if (!msgRecvQueue.empty())
        {
            int command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            LeaveCriticalSection(&my_winsec);
            return true;
        }
        my_mutex.unlock();
#endif
        return false;
    }

    void outMsgRecvQueue()
    {
        int command = 0;
        for (int i = 0; i < 100000; ++i)
        {
            bool result = outMsgLULProc(command);
            if (result == true)
            {
                cout << "outMsgRecvQueue()执行了，从容器中取出一个元素" << command << endl;
            }
            else
            {
                cout << "outMsgRecvQueue()执行了，但目前收消息队列中是空元素" << i << endl;
            }
        }
        cout << "end" << endl;
    }

private:
    std::list<int> msgRecvQueue;
    std::mutex my_mutex;
#ifdef _WINDOWSLJQ_
    //windows下叫临界区，类似于互斥量mutex
    CRITICAL_SECTION my_winsec;
#endif
};

int main()
{
    cout << "main threadid = " << std::this_thread::get_id() << endl;

    cout << "Main function execution finished! " << endl;
    return 0;
}