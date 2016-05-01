void downheap(int a[], int N, int k) 
{
    int j, v;
    v = a[k];
    while ( k <= N/2) 
    {
        j = k+k;
        if (j<k && a[j] < a[j+1]) j++;
        if (v >= a[j]) break;
        a[k] = a[j]; k = j;
    }
    a[k] = v;
}

void heapsort(int a[], int N) 
{
    int k, t;
    for (k = N/2; k>=1; k--) 
    	downheap(a, N, k);
    while (N>1) 
    {
        t = a[1]; a[1] = a[N] ; a[N] = t;
        downheap(a, --N, 1);
    }
}
