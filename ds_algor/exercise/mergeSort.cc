#include <iostream>
#include <stdio.h>
#include <time.h>
#include<stdlib.h>

using namespace std;


//合并两个 有序子数组
void merge(int a[], int temp[], int left, int right, int middle)
{
    //临时存储
    for(int index = left; index <= right; index++)
    {
        temp[index] = a[index];
    }

    int index1 = left;
    int index2 = middle+1;
    int i = left;

    while((index1 <= middle) && (index2 <= right) )
    {
        //取较小者 放入数组中
        if (temp[index1] < temp[index2])
            a[i++] = temp[index1++];
        else
            a[i++] = temp[index2++];
    }

    // 拷贝其余的元素
    while(index1 <= middle)
    {
        a[i++] = temp[index1++];
    }

    while(index2 <= right)
    {
        a[i++] = temp[index2++];
    }
}

void internalMergeSort(int a[], int temp[], int left, int right)
{
    if (left < right)
    {
        int middle = (left + right)/2;
        internalMergeSort(a, temp, left, middle);
        internalMergeSort(a, temp, middle+1, right);
        merge(a, temp, left, right, middle);
    }
}

void MergeSort(int a[], int len)
{
    int *temp = (int *)malloc(sizeof(int) * len);
    internalMergeSort(a, temp, 0, len-1);
    free(temp);
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

    MergeSort(a, testArrayLen);

    printArray(a, testArrayLen);

    return 0;
}
