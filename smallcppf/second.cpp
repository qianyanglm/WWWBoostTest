#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Solution
{
public:
    int getSum(int n)
    {
        int sum = 0;
        while (n)
        {
            sum += (n % 10) * (n % 10);
            n /= 10;
        }
        return sum;
    }

    bool isHappy(int n)
    {
        unordered_set<int> set;
        while (true)
        {
            int sum = getSum(n);
            if (sum == 1)
            {
                return true;
            }
            if (set.find(sum) != set.end())
            {
                return false;
            }
            else
            {
                set.insert(sum);
            }
            n = sum;
        }
    }
};

int main()
{
    unordered_map<char, int> map;
    // map['2'];
    cout << map['2'] << endl;
    return 0;
}