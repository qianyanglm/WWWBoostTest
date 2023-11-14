#include <iostream>
#include <ostream>

struct student
{
    char mark;
    long num;
    float score;
};

union test
{
    char mark;
    long num;
    float score;
};

enum test1 { code = 0,
             code1,
             code2 };

int main()
{
    std::cout << code1 << std::endl;
    return 0;
}