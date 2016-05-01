//m种硬币(每个硬币有不同的面值，存在S中)，求 组成面值为 N 的所有方法中 使用最少硬币个数是多少？
//minNum(i, j) = min( minNum(i-s(j)) + 1, minNum(i, j-1))   i:0->n j:1->m
int minNum(vector<int> S, int m, int n) {
    vector<int> table(n+1, INT::MAX);
    table[0] = 0;
    for(int i = 1; i <= n; i++)
        for(int j = 0; j < m; j++) {
            if( i >= s[j] && table[i] > table[i-s[j]]) 
                table[i] = table[i-s[j]] + 1; 
    }
    return table[n];
}

