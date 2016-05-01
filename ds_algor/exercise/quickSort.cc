#include <iostream>
#include <stdio.h>
#include <time.h>
#include<stdlib.h>

using namespace std;






void quickSort(int a[], int left, int right)
{
    int i,j;
    int pivot = a[right];
    i = left;
    j = right-1;
    if (left < right)
    {
        while (true)
        {
            //i 往右找第一个 大于等于 pivot的值
            while(a[i] < pivot && i != j)
                ++i;
            //j 往右找第一个 小于等于 pivot的值
            while(a[j] > pivot && i != j)
                --j;
            if (i < j)
                std::swap(a[i], a[j]);
            else
                break;
        }

        std::swap(a[i], a[right]);

        quickSort(a, left, i-1);
        quickSort(a, i+1, right);
    }
}


int partitionArray(vector<int> &nums, int k) 
{
    int right = 0;
    const int size = nums.size();
    for (int i = 0; i < size; ++i) 
    {
        if (nums[i] < k && i >= right) 
        {
            int temp = nums[i];
            nums[i] = nums[right];
            nums[right] = temp;
            ++right;
        }
    }

    return right;
}
    

int partition(int a[], int left, int right)
{
    int i = left;
    int j = right;
    int pivot = a[right];
    while (i != j)
    {
        //找第一个 比pivot大的值
        while(a[i] < pivot && i < j)
            i++;
        if (i < j)
        {
            a[j] = a[i];
            j--;
        }

        //找第一个 比pivot小的值
        while(a[j] > pivot && i < j)
            j--;
        if (i < j)
        {
            a[i] = a[j];
            i++;
        }
    }
    a[i] = pivot;
    return i;
}

void quickSort2(int a[], int left, int right)
{
    if (left >= right)
        return;

    int pivotIndex = partition(a, left, right);

    quickSort2(a, left, pivotIndex-1);
    quickSort2(a, pivotIndex+1, right);
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

    quickSort(a, 0, testArrayLen-1);

    printArray(a, testArrayLen);

    return 0;
}
