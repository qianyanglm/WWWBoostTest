// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//公众号：程序员速成   ，内含优质视频教程，欢迎关注

#include <atomic>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#define _CRTDBG_MAP_ALLOC

using namespace std;//后面再使用诸如std::cout时就可以简写成cout了；

class A
{
public:
    static void *operator new(size_t size);
    static void operator delete(void *phead);
    static int m_iCount;      //用于分配计数统计，每new一次+1
    static int m_iMallocCount;//用于统计malloc次数，每malloc一次+1
private:
    A *next;
    static A *m_FreePosi;    //总是指向一块可以分配出去的内存的首地址
    static int m_sTrunkCount;//一次分配多少倍该类的内存
};

void *A::operator new(size_t size)
{
    //A *ppoint = (A *)malloc(size); //不再用传统方式实现，而是用内存池实现
    //return ppoint;
    A *tmplink;
    if (m_FreePosi == nullptr)
    {
        //为空，我们要申请内存，申请的是很大一块内存
        size_t realsize = m_sTrunkCount * size;                //申请m_TrunkCount这么多倍的内存
        m_FreePosi = reinterpret_cast<A *>(new char[realsize]);//这是传统new，调用底层传统malloc
        tmplink = m_FreePosi;

        //把分配出来的这一大块内存链起来，供后续使用
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
    free(phead);

    // //free(phead);  //不再用传统方式实现，针对内存池有特别的实现
    // (static_cast<A *>(phead))->next = m_FreePosi;
    // m_FreePosi = static_cast<A *>(phead);
}

//----------------------------------------
int A::m_iCount = 0;
int A::m_iMallocCount = 0;

A *A::m_FreePosi = nullptr;
int A::m_sTrunkCount = 5;//一次分配5倍的该类内存作为内存池的大小

//int A::m_sTrunkCount = 500;

int main()
{
    {
        cout << "hello" << endl;
    }

    _CrtDumpMemoryLeaks();

    return 0;
}