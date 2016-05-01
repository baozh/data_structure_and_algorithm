// first sort, then fix a num and search in 2sum
set<vector<int> > find_triplets(vector<int> arr) {
  sort(arr.begin(), arr.end());
  set<vector<int> > triplets;
  vector<int> triplet(3);
  int n = arr.size();
  for (int i = 0;i < n; i++) {
    int j = i + 1;
    int k = n - 1;

    while (j < k) {
      int sum_two = arr[i] + arr[j];
      if (sum_two + arr[k] < 0) {
        j++;
      } else if (sum_two + arr[k] > 0) {
        k--;
      } else {
        triplet[0] = arr[i];
        triplet[1] = arr[j];
        triplet[2] = arr[k];
        triplets.insert(triplet);
        j++;
        k--;
      }
    }
  }
  return triplets;
}

// if not allow sort, use hash
bool three_sum(int nums[], int n, int target) {
    hash_map<int, int> arr_hash;
    for (int i =0; i< n; i++) {
        arr_hash[nums[i]] = i;
    }

    hash_map<int, int>::iterator iter;
    for (int i = 0; i< n-1; i++) {
        for (int j = i+1; j < n; j++) {
            int sum = nums[i] + nums[j];
            iter = arr_hash.find(target - sum);
            if (iter != arr_hash.end()) {
                // not allow the same element;
                if (iter->second > j) {
                  cout<<nums[i]<<" "<<nums[j]<<" "<<iter->first<<endl;
                  return true;
                }
            }
        }
    }
    return false;
}