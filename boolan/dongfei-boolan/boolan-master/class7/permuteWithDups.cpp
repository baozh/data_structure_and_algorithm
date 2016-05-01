//解法1: 在pszStr数组中，[nBegin,nEnd)中是否有数字与下标为nEnd的数字相等
bool IsSwap(char *pszStr, int nBegin, int nEnd) {
    for (int i = nBegin; i < nEnd; i++)
        if (pszStr[i] == pszStr[nEnd])
            return false;
    return true;
}

//k表示当前选取到第几个数, m表示共有多少数.
void PermutationHaveSameword(char *pszStr, int k, int m) {
    if (k == m) {
        printf("%s\n", pszStr);
    } else {
        for (int i = k; i <= m; i++) ｛ //第i个数分别与它后面的数字交换就能得到新的排列
            if (IsSwap(pszStr, k, i)) {
                swap(pszStr[k], pszStr[i]);
                PermutationHaveSameword(pszStr, k + 1, m);
                swap(pszStr[k], pszStr[i]);
            }
        }
    }
}

void main() {
    char* str = "abb";
    PermutationHaveSameword(str, 0, strlen(str));
}

// 解法2: 使用set去消重
void permuteHelper(int index, vector<int> &num, vector<vector<int>> &paths) {
    if(index > num.size()) {
        return;
    }

    if(index == num.size()) {
        paths.push_back(num);
    }
              
    unordered_set<int> used;
    for(int i = index; i < num.size(); i++) {
        // handle duplicates        
        if(used.count(num[i]))
            continue;

        // make choice
        swap(num, index, i);
        permuteHelper(index+1, num, paths);

        // backtracking
        swap(num, index, i);        
        used.insert(num[i]);
    }    
}

void swap(vector<int> &num, int start, int end) {
    int temp = num[start];
    num[start] = num[end];
    num[end] = temp;
}

vector<vector<int>> permuteUnique(vector<int> &num) {
    vector<vector<int>> paths;
    permuteHelper(0, num, paths);
    return paths;
}
