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
    int b = 6;
    int &&a = 7;
    cout << a << endl;
    return 0;
}