#include <iostream>
#include <memory>
#include <vector>
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <algorithm>
#include <crtdbg.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

class A
{
public:
    static void *operator new(size_t size);
    static void operator delete(void *phead);

    static int m_iCount;      //分配计数统计，new一次+1
    static int m_iMallocCount;//统计malloc次数，malloc一次+1

    A()
    {
        cout << "A的构造函数执行了" << endl;
    }

    ~A()
    {
        cout << "A的析构函数执行了" << endl;
    }

private:
    A *next;
    static A *m_FreePosi;    //总是指向一块可以分配出去的内存的首地址
    static int m_sTrunkCount;//一次分配多少倍该类的内存
};

void *A::operator new(size_t size)
{
    // std::cout << "A::operator new 被调用了!" << std::endl;
    // A *ppoint = (A *) malloc(size);
    // return ppoint;

    //不使用传统方式实现。使用内存池实现

    A *tmplink;
    if (m_FreePosi == nullptr)
    {
        //为空，我们要申请内存，申请的是很大一块内存
        size_t realsize = m_sTrunkCount * size;
        m_FreePosi = reinterpret_cast<A *>(new char[realsize]);
        tmplink = m_FreePosi;

        //把分配出来的内存链接起来，后续使用(单链表)
        for (; tmplink != &m_FreePosi[m_sTrunkCount - 1]; ++tmplink)
        {
            tmplink->next = tmplink + 1;
        }
        tmplink->next = nullptr;
        ++m_iMallocCount;
    }

    tmplink = m_FreePosi;
    m_FreePosi = m_FreePosi->next;
    ++m_iCount;
    return tmplink;
}

void A::operator delete(void *phead)
{
    A *p = static_cast<A *>(phead);
    // p->~A();// 先调用析构函数
    // 将 p 链接到 m_FreePosi
    p->next = m_FreePosi;
    m_FreePosi = p;
    --m_iCount;
    free(p);
}

int A::m_iCount = 0;
int A::m_iMallocCount = 0;

A *A::m_FreePosi = nullptr;
int A::m_sTrunkCount = 5;//一次分配5倍的该类内存作为内存池的大小

int main()
{
    {
        for (int i = 0; i < 15; ++i)
        {
            A *pa = new A();
            printf("%p\n", pa);
            delete pa;
        }
    }
    _CrtDumpMemoryLeaks();

    return 0;
}