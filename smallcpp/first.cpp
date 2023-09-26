#include <iostream>
using namespace std;

class Foo
{
public:
    void *operator new(std::size_t size)
    {
        std::cout << "operator new" << std::endl;
        return std::malloc(size);
    }
};

int main()
{
    Foo *m = new Foo;
    std::cout << sizeof(m) << std::endl;
    delete m;
    return 0;
}