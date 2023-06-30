#include "boost/lexical_cast.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    vector<int> a(2, 0);
    cout << a[0] << a[1] << endl;
    vector<int> b = a;
    cout << b[0] << b[1] << endl;

    return 0;
}