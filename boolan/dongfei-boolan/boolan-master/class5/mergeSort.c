void merge_sort( int array[], int helper[], int left, int right){
    if( left >= right )
        return;

    // divide and conquer: array will be divided into left part and right part
    // both parts will be sorted by the calling merge_sort
    int mid = right - (right - left) / 2;
    merge_sort( array, helper, left, mid );
    merge_sort( array, helper, mid  + 1, right);

    // now we merge two parts into one
    int helperLeft = left;
    int helperRight = mid + 1;
    int curr = left;
    for(int i = left; i <= right; i++)
        helper[i] = array[i];
    
    while( helperLeft <= mid && helperRight <= right ){
        if( helper[helperLeft] <= helper[helperRight] )
            array[curr++] = helper[helperLeft++];
        else
            array[curr++] = helper[helperRight++];
    }

    // left part has some large elements remaining. Put them into the right side
    while( helperLeft <= mid )
        array[curr++] = helper[helperLeft++];
}
