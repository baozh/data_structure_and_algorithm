#include <iostream>
#include <stdio.h>
#include <time.h>
#include<stdlib.h>

using namespace std;

void insertSort(int a[], int len)
{
    for (int i = 1; i<len ; i++)  //依次插入第i条记录
    {
        //a[j] 依次与前面的 有序子数据比较，如果a[j]比它小，就交换
        for (int j = i; j>0 ; j--)
        {
            if (a[j] < a[j-1])
            {
                std::swap(a[j], a[j-1]);
            }
            else
            {
                break;
            }
        }
    }
}


void bubbleSort(int a[], int len)
{
    for (int i = 1; i<len; i++)
    {
        //j: len-1 ~ i     每次都从最后一个元素开始向前冒泡，会把小的元素 滚到 前面的位置
        bool isSwap = false;
        for(int j = len-1; j >= i; j--)
        {
            if (a[j] < a[j-1])
            {
                std::swap(a[j], a[j-1]);
                isSwap = true;
            }
        }
        if (isSwap == false)  //优化：如果这一轮 没有发生交换，说明已经排好序了
            return;
    }
}




void selectSort(int a[], int len)
{
    for(int i=0; i<len -1; i++)
    {
        int smallest = i;
        for(int j = i+1; j<len; j++)  //从子序列[i+1 ~ len-1] 从找 最小的元素的index
        {
            if (a[j] < a[smallest])
                smallest = j;
        }
        std::swap(a[i], a[smallest]);
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

    selectSort(a, testArrayLen);

    printArray(a, testArrayLen);

    return 0;
}
