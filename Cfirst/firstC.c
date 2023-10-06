#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[16] = {0};
    int n = snprintf(str, sizeof(str), "12345%d\n67890%s1234", 777, "abc");
    printf("str=%s\n", str);
    printf("n=%d\n", n);
    return 0;
}