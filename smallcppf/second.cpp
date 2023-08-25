#include <iostream>
using namespace std;

class A
{
public:
    A() {}

    ~A() {}

    int m_i;
};

int main()
{
    char *ppoint = new char[55];
    memset(ppoint, 0, 55);
    delete[] ppoint;
    return 0;
}