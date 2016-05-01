class Solution {
public:
    int majorityNumber(vector<int> nums) {
        int candidate1, candidate2;
        int count1, count2;
        count1 = count2 = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (candidate1 == nums[i]) {
                count1 ++;
            } else if (candidate2 == nums[i]) {
                count2 ++;
            } else if (count1 == 0) {
                candidate1 = nums[i];
                count1 = 1;
            } else if (count2 == 0) {
                candidate2 = nums[i];
                count2 = 1;
            } else {
                count1--;
                count2--;
            }
        }
        count1 = count2 = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == candidate1) {
                count1++;
            } else if (nums[i] == candidate2) {
                count2++;
            }
        }    
        return count1 > count2 ? candidate1 : candidate2;
    }
};
