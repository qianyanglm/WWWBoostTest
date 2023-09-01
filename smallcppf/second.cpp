// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <algorithm>
#include <crtdbg.h>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

using namespace std;

// void mydeleter(string *pdel)
// {
//     delete pdel;
//     pdel = nullptr;
// }

void mydeleter2(string *pdel)
{
    delete pdel;
    pdel = nullptr;
}

int main()
{
    {
        int *arr = new int[10];
        arr[0] = 1;
        cout << arr[0] << endl;
        delete arr;
    }

    // _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}