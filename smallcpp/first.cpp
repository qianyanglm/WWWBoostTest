#include <iostream>
#include <memory>

class CT: public std::enable_shared_from_this<CT>
{
public:
    std::shared_ptr<CT> getself()
    {
        return shared_from_this();
    }
};

int main()
{
    std::shared_ptr<CT> pct1 = std::make_shared<CT>();
    std::shared_ptr<CT> pct2 = pct1->getself();

    std::cout << "pct1: " << pct1 << std::endl;
    std::cout << "pct1->getself(): " << pct1->getself() << std::endl;
    std::cout << "pct2: " << pct2 << std::endl;

    return 0;
}