#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>


using namespace std;

void printVec(const std::vector<int>& curSet)
{
    printf("\nsubSet:");
    std::vector<int>::const_iterator iter = curSet.begin();
    for (; iter != curSet.end(); iter++)
    {
        printf(" %d", *iter);
    }
}

void subsetHelper(std::vector<int>& curSet, int num[], int numLen, int pos)
{
    printVec(curSet);
    //求 当前curSet与 pos之后元素的 各个子集
    for (int i=pos; i<numLen; i++)
    {
    	//每次迭代(一个for循环求的) 往curSet推入一个元素，求 curSet.size()+1个元素的组合.
        curSet.push_back(num[i]);
        subsetHelper(curSet, num, numLen, i+1);
        curSet.pop_back();
    }
}

void subset(int num[], int len)
{
    std::sort(&num[0], &num[0] + len);
    std::vector<int> curSet;
    subsetHelper(curSet, num, len, 0);
}

void subsetUniqueHelper(std::vector<int>& curSet, int num[], int numLen, int pos)
{
    printVec(curSet);
    for (int i=pos; i<numLen; i++)
    {
        if (i>0 && i!= pos && num[i] == num[i-1])
            continue;

        curSet.push_back(num[i]);
        subsetUniqueHelper(curSet, num, numLen, i+1);
        curSet.pop_back();
    }
}

void subsetUnique(int num[], int len)
{

    std::sort(&num[0], &num[0] + len);
    std::vector<int> curSet;
    subsetUniqueHelper(curSet, num, len, 0);
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


//从7个元素中 取出3个元素的  所有组合
void test()
{
	//思路：对{ 1, 1, 1, 0, 0, 0, 0 }做全排列，取出"1"对应位置的值
	int values[] = { 1, 2, 3, 4, 5, 6, 7 };
	int elements[] = { 1, 1, 1, 0, 0, 0, 0 };
	const size_t N = sizeof(elements)/sizeof(elements[0]);
	assert(N == sizeof(values)/sizeof(values[0]));
	std::vector<int> selectors(elements, elements + N);

	int count = 0;
	do
	{
		std::cout << ++count << ": ";
		for (size_t i = 0; i < selectors.size(); ++i)
		{
		  if (selectors[i])
		  {
			std::cout << values[i] << ", ";
		  }
		}
		std::cout << std::endl;
	} while (prev_permutation(selectors.begin(), selectors.end()));
}


int main()
{
    const int testArrayLen = 5;
    int a[testArrayLen];

    for(int i = 0; i<testArrayLen; i++)
    {
        a[i] = i+1;
    }

    printArray(a, testArrayLen);

    subset(a, testArrayLen);

    a[0] = 1;
    a[1] = 2;
    a[2] = 2;
    printArray(a, 3);
    subsetUnique(a, 3);
    return 0;
}
