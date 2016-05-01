int partition( int array[], int left, int right ) {
    int pivot = array[right];
    while( left != right ){
        while( array[left] < pivot && left < right)
            left++;
        if (left < right) {
            swap( array[left], array[right--]);
        }
        while( array[right] > pivot && left < right)
            right--;
        if( left < right )
            swap( array[left++], array[right]);
    }
    return left;
}

//在无序数组中 查找第k大的数组元素，O(n)
int quick_select(int array[], int left, int right, int k) {
    if ( left >= right )
        return array[left];
    int index = partition(array, left, right);
    int size = index - left + 1;
    if ( size == k )
        return array[left + k - 1]; // the pivot is the kth largest element
    else if ( size > k )   //从左半部分中找
        return quick_select(array, left, index - 1, k);
    else      //从右半部分中找
        return quick_select(array, index + 1, right , k - size);
}
