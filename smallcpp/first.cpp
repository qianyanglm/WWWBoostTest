#include <iostream>
#include <thread>
#include <vector>
using namespace std;

vector<int> g_v = {1, 2, 3};

void myprint(int inum)
{
    cout << "id为" << std::this_thread::get_id() << "的线程打印g_v值" << g_v[0] << g_v[1] << g_v[2] << endl;
    return;
}

int main()
{
    vector<thread> mythreads;
    for (int i = 0; i < 5; ++i)
    {
        mythreads.push_back(thread(myprint, i));
    }
    for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
    {
        iter->join();
    }
    cout << "main主函数执行结束！" << endl;

    return 0;
}