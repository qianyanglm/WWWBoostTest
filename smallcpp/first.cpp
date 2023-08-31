#include <iostream>
#include <memory>
using namespace std;

class A
{
public:
    A()
    {}

    ~A() {}
};

int main()
{
    std::unique_ptr<A[]> ptrarray(new A[10]);
    // std::shared_ptr<A> ptr(new A[10]);
    shared_ptr<A> ptr2(new A[10], [](A *p)
                       { delete[] p; });
    std::shared_ptr<A[]> ptr3(new A[10]);
    return 0;
}