int binarySearch(int *array, int left, int right, int value)  
{
    while (left<=right)          //循环条件，适时而变  
    {  
        int middle = left + (right-left)/2;  //使用“(left + right) / 2”可能会造成栈溢出  

        if (array[middle]>value)  
        {  
            right =middle-1;   //right赋值，适时而变  
        }   
        else if(array[middle]<value)  
        {  
            left=middle+1;  
        }  
        else  
            return middle;    
        //可能会有读者认为刚开始时就要判断相等，但毕竟数组中不相等的情况更多  
        //如果每次循环都判断一下是否相等，将耗费时间  
    }

    return -1;  
}  

// recursion
int binarySearch(int *array, int left, int right, int value) 
{
    if (left > right) 
    {
        // value not found
        return -1;
    }

    int mid = left + (right - left) / 2;
    if (array[mid] == value) 
    {
        return mid;
    } 
    else if (array[mid] < value) 
    {
        return binarySearch(array, mid + 1, right, value);
    } else 
    {
        return binarySearch(array, left, mid - 1, value);
    }
}

