class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int> > result;
        if (nums.empty())
            return result;

        sort(nums.begin(), nums.end());
        vector<int> list;
        dfs(nums, 0, list, result);

        return result;
    }

private:
    void dfs(vector<int>& nums, int pos, vector<int> &list,
             vector<vector<int> > &ret) {

        ret.push_back(list);

        for (int i = pos; i < nums.size(); ++i) {
            list.push_back(nums[i]);
            dfs(nums, i + 1, list, ret);
            list.pop_back();
        }
    }
};

// 解法2:位运算
void Combination(char *str) {
    int len=strlen(str); 
    ////遍历所有的情况，1<<len就等于2^len，遍历1 ~ 2^len-1
    for(int cur=1; cur < (1<<len); cur++)  {
        for(int j=0; j < len; j++) { //遍历所有的字符
            if(cur & (1 << j))  //判断哪一位为1，即输出该位
                cout << str[j]; 
        }
        cout << endl;  //一种情况结束
    }
}
