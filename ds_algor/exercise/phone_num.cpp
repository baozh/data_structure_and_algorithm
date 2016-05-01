class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> ret;
        /* for this question, we need to create a look-up dictionary */
        vector<string> dic;
        string curr;
        dic.push_back(" ");
        dic.push_back(" ");
        dic.push_back("abc");
        dic.push_back("def");
        dic.push_back("ghi");
        dic.push_back("jkl");
        dic.push_back("mno");
        dic.push_back("pqrs");
        dic.push_back("tuv");
        dic.push_back("wxyz");
        combinations(ret, curr, digits, dic, 0);
        return ret;
    }

    void combinations(vector<string>& ret, string curr, string digits, vector<string> dic, int level) {
        if (level == digits.size()) {
            ret.push_back(curr);
            return;
        }

        int index = digits[level]-'0';
        for (int i = 0; i < dic[index].size(); ++i) {
            curr.push_back(dic[index][i]);
            combinations(ret, curr, digits, dic, level+1);
            curr.pop_back();
        }
    }
};
