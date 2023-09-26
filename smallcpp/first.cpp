#include <stdio.h>
#include <stdlib.h>

void *operator new(size_t sz)
{
    printf("operator new:%d Bytes\n", sz);
    void *m = malloc(sz);
    if (!m)
        puts("out of memory");
    return m;
}

void operator delete(void *m)
{
    puts("operator delete");
    free(m);
}

class S
{
    int i[100];

public:
    S() { puts("S:S()"); }

    ~S() { puts("~S::S()"); }
};

int main()
{
    puts("(1)Creating & Destroying an int");
    int *p = new int(47);
    delete p;
    puts("(2)Creating & Destroying an S");
    S *s = new S;
    delete s;
    puts("(3)Creating & Destroying S[3]");
    S *sa = new S[3];
    delete[] sa;
    return 0;
}