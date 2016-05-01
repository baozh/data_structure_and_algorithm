// use it in quick sort
int partition(int arr[], int n, int pivot) {
      int i = 0, j = n-1;
      int tmp;
     
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;

            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      }
      return i;
}

// another compare
int partition(int v[], int n, int key) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        if (v[i] >= key) {
            swap(v[i], v[j]);
            --j;
        } else
            ++i;

        if (v[j] < key) {
            swap(v[i], v[j]);
            ++i;
        } else
            --j;
    }
    return i;
}
