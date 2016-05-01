void random_permutation(int a[], int n) {
    int i;
    for (i=n; i>1; i--)
        swap(&a[i-1],&a[random_int(0,i-1)]);
}


