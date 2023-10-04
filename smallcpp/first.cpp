#include <iostream>
using namespace std;

void test(int arg[])
{
    for (unsigned int i = 0; i < sizeof(arg); ++i)
    {
        cout << arg[i] << endl;
    }
}

int main()
{
    int a[10] = {
            1,
            2,
            3,
    };
    test(a);
    return 0;
}