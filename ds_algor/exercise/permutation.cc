#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>


using namespace std;



void permutationRecursion(string & s, size_t begin)
{
    if(begin == s.length())
    {
        cout << s << '\n';
        return;
    }

    for(size_t i = begin; i < s.length(); i++)
    {
        //每次迭代 固定使 a[begin] 与 其它元素交换，再求 交换后的序列的 子排列
        std::swap(s[begin], s[i]);
        permutationRecursion(s, begin+1);
        std::swap(s[begin], s[i]);
    }
}

void permutaionUseAlgo(string& s, size_t begin)
{
    std::vector<char> vec(s.begin(), s.end());

    int count = 0;
    do
    {
        std::cout << ++count << ": ";
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<char>(std::cout, ", "));
        std::cout << std::endl;
    } while (next_permutation(vec.begin(), vec.end()));
}


int main()
{
    string s = "abcd";
    //在求子序列之前， 需要将 s 排序
    permutationRecursion(s, 0);

    return 0;
}




