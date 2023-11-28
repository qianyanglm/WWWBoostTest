class Parent
{
protected:
    int protected_member;
};

class Child: public Parent
{
public:
    void access()
    {
        protected_member = 10;// 可以访问 protected_member
    }
};