// map, time: O(n)
vector<int> addsToTarget(vector<int> &numbers, int target) {
    unordered_map<int, int> numToIndex;
    vector<int> vi(2);
    for (auto it = numbers.begin(); it != numbers.end(); it++) {
        if (numToIndex.count(target - *it)) {
            vi[0] = numToIndex[target - *it] + 1;
            vi[1] = (int)(it - numbers.begin()) + 1;
            return vi;
        }
        numToIndex[*it] = (int)(it - numbers.begin());
    }
}

// quicksort, time: O(nlgn),
bool hasArrayTwoCandidates(int A[], int arr_size, int sum) {
    int l, r;

    /* Sort the elements */
    quickSort(A, 0, arr_size-1);

    /* Now look for the two candidates in the sorted
       array*/
    l = 0;
    r = arr_size-1;
    while (l < r) {
         if (A[l] + A[r] == sum)
              return 1;
         else if (A[l] + A[r] < sum)
              l++;
         else // A[i] + A[j] > sum
              r--;
    }
    return 0;
}