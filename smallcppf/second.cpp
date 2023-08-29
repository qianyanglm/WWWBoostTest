// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

class CA;
class CB;

class CA
{
public:
    // shared_ptr<CB> m_pbs;
    weak_ptr<CB> m_pbs;

    ~CA()
    {
        cout << "~A()执行了" << endl;
    }
};

class CB
{
public:
    shared_ptr<CA> m_pas;

    // weak_ptr<CA> m_pas;

    ~CB()
    {
        cout << "~B()执行了" << endl;
    }
};

void delete1(int *p)
{
    delete p;
}

int main()
{
    {
        int *pi = new int(100);
        shared_ptr<int> p1(pi, delete1);
        cout << sizeof(p1) << endl;
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}