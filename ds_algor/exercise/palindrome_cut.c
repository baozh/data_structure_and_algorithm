//对一个字符串 分割成 回文，找到它的最小分割数
//判断 子串(i->j)是否是回文：
//    = (value(i) == value(j)) && (子串(i+1 -> j-1)是回文 || j-1 <= 1)
//最小分割数：
//  minCut(i) = min( minCut(j+1)+1)  j从i到n 开始计算，找 ` minCut(j+1)+1` 的最小值
int minCut(string s) {
    if(s.empty())   return 0;
    vector<vector<bool> > palin(s.size(), vector<bool>(s.size(),false));
    vector<int> minCut(s.size()+1,0);
    for(int i = 0; i <= s.size(); i++)
        minCut[i] = s.size() - i - 1;
       
    for(int i = s.size() - 1; i >= 0; i--) {
        for(int j = i; j < s.size();++j) {              
            if(s[i] == s[j] && ( j - i <= 1 || palin[i+1][j-1] ) ) {
                palin[i][j] = true;
                minCut[i] = min(minCut[j+1]+1, minCut[i]);
            }              
        }
    }
       
    return minCut[0];     
}

