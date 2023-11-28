#include <assert.h>
#include <iostream>
#include <string>
using namespace std;

template<class type>
class ablist
{
public:
    int GetLength()
    {
        return length;
    }

    virtual type GetElements(int index);
    virtual bool Expand(int) = 0;
    virtual void Empty() = 0;
    virtual bool InsertTarget(type value, int index) = 0;
    virtual type RemoveElement(int index) = 0;
    virtual type RemoveTarget(type target) = 0;

protected:
    int length = 0;
};

template<class type>
class SeqList: public ablist<type>
{
protected:
    type *elements;
    int maxsize;

public:
    SeqList(int maxs = 10);
    SeqList<type> &Copy(SeqList<type> &s);
    bool Expand(int);
    bool SetElements(type x, int index);
    type GetElements(int index);

    void Empty()
    {
        if (elements != NULL) delete[] elements;
    }

    type RemoveElement(int index);
    type RemoveTarget(type target);
    bool InsertTarget(type value, int index);

    SeqList<type> &operator=(SeqList<type> &s)
    {
        Copy(s);
        return *this;
    }

    // friend ostream& operator<<(ostream&,  SeqList<type>&);
    ~SeqList()
    {
        if (elements != NULL) delete[] elements;
    }
};

template<class type>
SeqList<type>::SeqList(int maxs)
{
    maxsize = maxs > 10 ? maxs : 10;
    this->length = 0;
    elements = NULL;
    elements = new type[maxs];
    assert(elements != NULL);
}

// 将原顺序表赋值拷贝给新顺序表
template<class type>
SeqList<type> &SeqList<type>::Copy(SeqList<type> &s)
{
    if (elements)
        delete[] elements;
    maxsize = 0;
    length = 0;
    elements = new type[s.maxsize];

    if (elements)
    {
        maxsize = s.maxsize;
        length = s.length;
        // memmove 将s.elements的内容复制到elements,长度为size
        memmove(elements, s.elements, sizeof(type) * maxsize);
    }
    return *this;
}

template<class type>
bool SeqList<type>::Expand(int newmaxsize)
{
    type *p, *q;// 新建两个指针,一个用来指向新list,一个保存原list
    if (newmaxsize <= maxsize)
    {
        cout << "New m