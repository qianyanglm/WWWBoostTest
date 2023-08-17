#include <type_traits>

int add(int x, double y)
{
    return x + static_cast<int>(y);
}

int main()
{
    std::result_of<decltype(add) &(int, double)>::type result = 0;
    static_assert(std::is_same<decltype(result), int>::value, "result type should be int");
    return 0;
}