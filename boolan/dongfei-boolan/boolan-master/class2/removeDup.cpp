class Solution {
public:
    /**
     * @param A: a list of integers
     * @return : return an integer
     */
    int removeDuplicates(vector<int> &nums) 
    {
        if (nums.empty()) 
        {
            return 0;
        }

        int size = 0;
        for (vector<int>::size_type i = 0; i != nums.size(); ++i) 
        {
            if (nums[i] != nums[size]) 
            {
                nums[++size] = nums[i];
            }
        }
        return ++size;
    }
};
