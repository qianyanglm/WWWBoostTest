#include <stdio.h>

void test(int arg[])
{
    for (unsigned int i = 0; i < sizeof(arg); ++i)
    {
        printf("%d\n", arg[i]);
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