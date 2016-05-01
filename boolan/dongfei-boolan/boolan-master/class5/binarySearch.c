// recursion
int binarySearch(int *array, int left, int right, int value) {
    if (left > right) {
        // value not found
        return -1;
    }

    int mid = left + (right - left) / 2;
    if (array[mid] == value) {
        return mid;
    } else if (array[mid] < value) {
        return binarySearch(array, mid + 1, right, value);
    } else {
        return binarySearch(array, left, mid - 1, value);
    }
}

// iterative. From Programming Pearls
int binary_search(const int a[], const int size, const int val) {
  int left = 0;
  int right = size-1;
 /* invariant: if a[i]==val for any i, then left <= i <= right */
  while (left <= right) {
    int i = left + (right-left)>>1;
    if (val == a[i]) {
      return i;
    } else if (val < a[i]) {
      right = i-1;
    } else { /* val > a[i] */
      left = i+1;
    }
  }
  return -1;
}