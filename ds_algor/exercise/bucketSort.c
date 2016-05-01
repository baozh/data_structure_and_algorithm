void BucketSort(int array[], int n, int max)
{
    // array of length n，all records in the range of [0,max)
    int tempArray[n];
    int i;
    for (i = 0; i < n; i++)
        tempArray[i] = array[i];

    int count[max];    // buckets
    memset(count, 0, max * sizeof(int));

    for (i = 0; i < n; i++)	// put elements into the buckets
        count[array[i]]++;
    for (i = 1; i < max; i++)
        count[i] = count[i-1] + count [i];  // count[i] saves the starting index (in array) of value i+1

    // for value tempArray[i], the last index should be count[tempArray[i]]-1
    for (i = n-1; i >= 0; i--)
        array[--count[tempArray[i]]] = tempArray[i];
}
