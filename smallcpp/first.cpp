#include <future>
#include <iostream>
using namespace std;

void mythread(std::promise<int> &tmp, int calc)
{
    cout << "mythread() start threadid = " << this_thread::get_id() << endl;
    calc++;
    calc *= 10;
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura);
    int result = calc;
    tmp.set_value(result);
    cout << "mythread() end threadid = " << std::this_thread::get_id() << endl;
}

int main()
{
    cout << "main threadid = " << this_thread::get_id() << endl;
    std::promise<int> myprom;
    std::thread t1(mythread, std::ref(myprom), 180);
    // t1.join();
    std::future<int> fu1 = myprom.get_future();
    auto result = fu1.get();

    cout << "result = " << result << endl;

    cout << "The main function line is over!" << endl;

    return 0;
}