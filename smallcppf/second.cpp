#include <iostream>
using namespace std;

class A
{
public:
    A() { cout << "A" << endl; }

    ~A() {}

    int m_i;
};

int main()
{
    string *mystr2 = new string();
    int *pointi3 = new int();
    int *pointi1 = new int;
    A *pa1 = new A;
    A *pa2 = new A();
    string *mystr23 = new string(5, 'a');
    auto mystr3 = new auto(mystr2);
    string **mystr4 = new (string *)(mystr2);
    return 0;
}