int rotated_binary_search(int A[], int N, int key) {
  int L = 0;
  int R = N - 1;

  while (L <= R) {
    // Avoid overflow, same as M=(L+R)/2
    int M = L + ((R - L) >> 1);
    if (A[M] == key) return M;

    // the bottom half is sorted
    if (A[L] <= A[M]) {
      if (A[L] <= key && key < A[M])
        R = M - 1;
      else
        L = M + 1;
    }
    // the upper half is sorted
    else {
      if (A[M] < key && key <= A[R])
        L = M + 1;
      else
        R = M - 1;
    }
  }
  return -1;
}

// recursion implementation
int searchInRotatedArrayHelper(int array[], int left, int right, int target) {
    if (left > right) {
        // exit: target not found
        return -1;
    }

    int mid = right - (right - left) / 2;
    if (array[mid] == target) {
        // exit: target found
        return mid;
    }

    if (array[left] <= array[mid]) {
        // left half is completely sorted
        if (target >= array[left] && target <= array[mid]) {
            // target in the sorted part
            return searchInRotatedArrayHelper(array, left, mid - 1, target);
        } else {
            // target not in the sorted part
            return searchInRotatedArrayHelper(array, mid + 1, right, target);
        }
    } else {
        // right half is completely sorted
        if (target >= array[mid] && target <= array[right]) {
            // target in the sorted part
            return searchInRotatedArrayHelper(array, mid + 1, right, target);
        } else {
            // target not in the sorted part
            return searchInRotatedArrayHelper(array, left, mid - 1, target);
        }
    }
}

int searchInRotatedArray(int array[], int n, int target) {
    return searchInRotatedArrayHelper(array, 0, n - 1, target);
}
