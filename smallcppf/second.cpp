// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <algorithm>
#include <crtdbg.h>
#include <iostream>
#include <memory>
#include <stdlib.h>

using namespace std;

class A
{
public:
    A()
    {
    }

    ~A()
    {
    }
};

auto myfunc()
{
    return std::make_unique<string>("I love China!");
}

void mydeleter(string *pdel)
{
    delete pdel;
    pdel = nullptr;
}

int main()
{
    {
        unique_ptr<string> ps(new string("I"));
        shared_ptr<string> ps2 = move(ps);
        cout << *ps2 << endl;
    }

    // _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}