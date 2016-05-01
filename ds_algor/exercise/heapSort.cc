#include <iostream>
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include <algorithm>


using namespace std;

bool greater_cmp(const int &a, const int &b) //等价于: greater<int>()
{
    return a>b;
}

void heapSort(int a[], int len)
{
    vector<int> ivec(a, a+len);

    make_heap(ivec.begin(), ivec.end(), greater_cmp);

    int i = 0;
    while(ivec.empty() == false)
    {
        pop_heap(ivec.begin(), ivec.end(), greater_cmp);//先pop,然后在容器中删除
        a[i++] = ivec.back();
        ivec.pop_back();
    }
}


void printArray(int a[], int len)
{
    std::cout<<endl;
    std::cout<<"array:";
    for(int i = 0; i<len; i++)
    {
        if (i == 0)
        {
            std::cout << a[i];
        }
        else
        {
            std::cout << ", "<< a[i];
        }
    }
}


int main()
{
    srand( (unsigned)time( NULL ) );

    const int testArrayLen = 30;
    int a[testArrayLen];

    for(int i = 0; i<testArrayLen; i++)
    {
        a[i] = rand() % 10000;
    }

    printArray(a, testArrayLen);

    heapSort(a, testArrayLen);

    printArray(a, testArrayLen);

    return 0;
}
