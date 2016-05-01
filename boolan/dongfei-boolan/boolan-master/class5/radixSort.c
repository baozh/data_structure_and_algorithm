void RadixSort(int Array[], int n, int digits, int radix)
{
    // n is the length of the array
    // digits is the number of digits
    int  *TempArray = new int[n];
    int *count = new int[radix]; // radix buckets
    int i, j, k;
    int Radix = 1; // radix modulo, used to get the ith digit of Array[j]
    // for ith digit
    for (i = 1; i <= digits; i++)  {
        for (j = 0; j < radix; j++)
            count[j] = 0;            // initialize counter
        for (j = 0; j < n; j++)	 {
            // put elements into buckets
            k = (Array[j] / Radix) % radix;  // get a digit
            count[k]++;
        }
        
        for (j = 1; j < radix; j++) {
            // count elements in the buckets
            count[j] = count[j-1] + count[j];
        }

        // bucket sort
        for (j = n-1; j >= 0; j--)  {
            k = (Array[j] / Radix ) % radix;
            count[k]--;
            TempArray[count[k]] = Array[j];
        }
        for (j = 0; j < n; j++) {
            // copy data back to array
            Array[j] = TempArray[j];
        }
        Radix *= radix;      // get the next digit
    }
}