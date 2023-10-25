#include <stdio.h>
#include <stdlib.h>

int main(int argc1, char *argv2[])
{
    printf("参数个数=%d\n", argc1);
    for (int i = 0; i < argc1; ++i)
    {
        printf("参数%s\n", argv2[i]);
    }
    return 0;
}