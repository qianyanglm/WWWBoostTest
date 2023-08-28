// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <iostream>

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

int main()
{
    // shared_ptr<int> myp(new int(100));
    // int icount = myp.use_count();
    // cout << icount << endl;

    int *a = (int *) malloc(sizeof(int));
    free(a);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}