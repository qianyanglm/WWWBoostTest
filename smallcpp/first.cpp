// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <iostream>
#include <memory>

using namespace std;
using namespace std;

class A
{
public:
    A()
    {
        // std::cout << "A" << std::endl;
    }

    // ~A() {}
};

void myfunc(shared_ptr<int> ptmp)
{
    return;
}

shared_ptr<int> myfunc2(shared_ptr<int> &ptmp)
{
    return ptmp;
}

void myDeleter(int *p)
{
    cout << "hello" << endl;
    // delete p;
}

int main()
{
    shared_ptr<int> pi = make_shared<int>(100);
    weak_ptr<int> piw(pi);
    weak_ptr<int> piw2(piw);
    auto pi2 = piw2.lock();

    // pi.reset();
    cout << pi2 << endl;
    cout << pi.use_count() << endl;
    cout << piw.use_count() << endl;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

    // return 0;
}