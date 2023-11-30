#include <assert.h>
#include <iostream>
#include <string.h>
using namespace std;

template<class type>
class ablist
{
public:
    int GetLength()
    {
        return length;
    }

    virtual type GetElements(int index) = 0;
    virtual void Empty() = 0;
    virtual bool SetElements(type x, int index) = 0;
    virtual bool InsertTarget(type value, int index) = 0;
    // virtual type RemoveElement(int index)=0;
    // virtual type RemoveTarget(type target)=0;

protected:
    int length;
};

template<class type>
class SeqList: public ablist<type>
{
    // protected:
public:
    type *elements;
    int maxsize;

public:
    SeqList(int maxs = 10);
    ~SeqList();
    //     if(elements) delete []elements;
    // }
    SeqList<type> &Copy(SeqList<type> &s);
    bool Expand(int);
    bool SetElements(type x, int index);
    type GetElements(int index);

    void Empty()
    {
        if (elements) delete[] elements;
    }

    // type RemoveElement(int index);
    // type RemoveTarget(type target);
    bool InsertTarget(type value, int index);

    SeqList<type> &operator=(SeqList<type> &s)
    {
        Copy(s);
        return *this;
    }

    // friend basic_ostream<char>& operator<<(ostream&,  SeqList<type>&);
    // friend ostream& operator<<(ostream&,  SeqList<type>&);
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

template<class type>
SeqList<type>::~SeqList()
{
    if (elements != nullptr)
    {
        delete[] elements;
        elements = NULL;
    }
}

// 将原顺序表赋值拷贝给新顺序表
template<class type>
SeqList<type> &SeqList<type>::Copy(SeqList<type> &s)
{
    if (elements)
        delete[] elements;
    maxsize = 0;
    this->length = 0;
    elements = new type[s.maxsize];

    if (elements)
    {
        maxsize = s.maxsize;
        this->length = s.length;
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
        cout << "New maxsize less than original size!\n";
        return false;
    }
    p = new type[newmaxsize];
    if (!p)
    {
        cout << "Cannot expand the seqlist.\n";
        return false;
    }
    memmove(p, elements, sizeof(type) * this->length);
    q = elements;
    elements = p;// 将新指针的地址赋给elements
    delete[] q;  // 释放q的内存区
    return true;
}

template<class type>
bool SeqList<type>::SetElements(type x, int index)
{
    if (index > maxsize)
    {
        cout << "Index is out of range!\n";
        return false;
    }
    if (index >= this->length) this->length = index + 1;
    elements[index] = x;
    return true;
}

template<class type>
type SeqList<type>::GetElements(int index)
{
    assert(index < maxsize);
    return elements[index];
}

template<class type>
bool SeqList<type>::InsertTarget(type x, int i)
{
    int j;
    if (i >= maxsize)
    {
        if (Expand(maxsize * 2))
        {
            elements[this->length] = x;
        }
        else
        {
            cout << "Cannot expand the list!\n";
            return false;
        }
    }
    else if (i >= this->length)
    {
        elements[this->length] = x;
    }
    else
    {
        for (j = this->length; j > i; j--)
        {
            elements[j] = elements[j - 1];
            elements[j] = x;
        }
    }
    this->length++;
    return true;
}

void printList(SeqList<int> &s)
{
    cout << s.elements << endl;
    cout << "the element are following: \n";
    for (int i = 1; i < s.GetLength(); i++)
    {
        cout << s.GetElements(i) << endl;
    }
}

void test01()
{
    SeqList<int> s1(10);
    s1.SetElements(0, 1);
    s1.SetElements(1, 2);
    s1.SetElements(2, 3);
    cout << s1.elements << endl;
    printList(s1);
    s1.InsertTarget(9, 4);
    // printList(s1);
    // s1.Empty();
    // printList(s1);
}

int main()
{
    test01();
    // SeqList<int> s1(10);
    // s1.SetElements(0, 1);
    // s1.SetElements(1, 2);
    // s1.SetElements(2, 3);
    // printList(s1);
    // system("pause");
    return 0;
}