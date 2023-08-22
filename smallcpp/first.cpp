#include <future>
#include <iostream>
#include <thread>
using namespace std;

int mythread(int mypar)
{
    cout << "mythread() start threadid = " << std::this_thread::get_id() << endl;
    std::chrono::milliseconds dura(1000);
    std::this_thread::sleep_for(dura);
    //    cout << "mythread() end threadid = " << std::this_thread::get_id() << endl;
    return 5;
}

void mythread2(std::future<int> &tmpf)
{
    cout << "mythread2()start"
         << " threadid = " << std::this_thread::get_id() << endl;
    auto result = tmpf.get();
    //    auto result1 = tmpf.get();
    cout << "mythread2 result =" << result << endl;
    return;
}

int myThread(int num)
{
    cout << "Thread start id = " << this_thread::get_id() << endl;
    chrono::milliseconds time(1000);
    this_thread::sleep_for(time);
    cout << "Thread end id = " << this_thread::get_id() << endl;
    return num * 2;
}

int main()
{
    cout << "main threadid = " << std::this_thread::get_id() << endl;
    std::packaged_task<int(int)> mypt(myThread);
    std::thread t1(std::ref(mypt), 1);
    t1.join();
    std::shared_future<int> result = mypt.get_future();
    //    std::thread t2(mythread2, std::ref(result));
    //    t2.join();
    //    std::thread t3(mythread2, std::ref(result));
    //    t3.join();
    cout << "result = " << result.get() << endl;
    cout << "result = " << result.get() << endl;

    cout << "Main function execution finished! " << endl;
    return 0;
}