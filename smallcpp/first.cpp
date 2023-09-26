#include <atomic>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>
#define _CRTDBG_MAP_ALLOC

using namespace std;//后面再使用诸如std::cout时就可以简写成cout了；

class myallocator
{
public:
    void *allocate(size_t size)
    {
        obj *tmplink;
        if (m_FreePosi == nullptr)
        {
            size_t realsize = m_sTrunkCout * size;
            m_FreePosi = (obj *) malloc(realsize);
            vectptr.push_back(m_FreePosi);
            tmplink = m_FreePosi;

            //大块内存里的小内存链接起来,方便后续分配使用
            for (int i = 0; i < m_sTrunkCout - 1; ++i)
            {
                tmplink->next = (obj *) ((char *) tmplink + size);
                tmplink = tmplink->next;
            }
            tmplink->next = nullptr;
        }

        //如果当前有可用的内存块，就直接从已经分配好的内存块中分配内存
        //将tmplink指向当前可用内存块，并将m_FreePosi移动到下一个可用内存块，方便下次分配使用
        tmplink = m_FreePosi;
        m_FreePosi = m_FreePosi->next;
        return tmplink;
    }

    //释放内存
    void deallocate(void *phead)
    {
        // //将传入的内存块的 next 指针设置为当前可用内存块，表示释放的内存块成为可用内存
        // ((obj *) phead)->next = m_FreePosi;
        //
        // //将 m_FreePosi 指向传入的内存块，以便下次分配时可以使用。
        // m_FreePosi = (obj *) phead;
        // // if (phead == temP)
        // // {
        // //     free(temP);
        // // }

        for (auto iter = vectptr.begin(); iter != vectptr.end(); iter++)
        {
            free(*iter);
        }
        vectptr.clear();
    }

private:
    struct obj
    {
        struct obj *next;//嵌入式指针
    };

    int m_sTrunkCout = 5;     //分配够5倍内存作为内存池大小
    obj *m_FreePosi = nullptr;//定义嵌入式指针，跟踪可用内存块
    vector<obj *> vectptr;    //定义嵌入式指针容器来保存malloc分配的内存地址，然后再释放
};

class A
{
public:
    int m_i;
    int m_j;

public:
    static myallocator myalloc;

    static void *operator new(size_t size)
    {
        return myalloc.allocate(size);
    }

    static void operator delete(void *phead)
    {
        return myalloc.deallocate(phead);
    }
};

myallocator A::myalloc;//类A外定义这个静态成员变量

int main()
{
    {
        A *mypa[100];
        for (int i = 0; i < 15; ++i)
        {
            mypa[i] = new A();
            printf("%p\n", mypa[i]);
        }
        for (int i = 0; i < 15; ++i)
        {
            delete mypa[i];
        }
        cout << sizeof(A::myalloc) << endl;
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

    return 0;
}