//m种硬币(每个硬币有不同的面值，存在S中)，求 组成面值为 N 有多少种不同的方法
//ways(i,j) = ways(i-s(j), j) + ways(i, j-1)   i从0->N j从1->m
//i:当前组成的面值的和   j:当前使用的硬币的种类的个数
int countWays(vector<int> S, int m, int n) 
{
    vector<int> table(n+1, 0);
    table[0] = 1;
    for(int i = 1; i <= n; i++)
        for(int j = 0; j < m; j++)
            table[i] += (i-s[j] >= 0) ? table[i-S[j]] : 0;
    return table[n];
}

