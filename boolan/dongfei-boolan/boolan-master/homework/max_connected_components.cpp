// http://www.careercup.com/question?id=5998719358992384
#include <vector>
using namespace std;

class Solution {
public:
    int maxConnectedComponent(vector<vector<int> > &v) {
        n = (int)v.size();
        if (n == 0) {
            return 0;
        }
        m = (int)v[0].size();
        if (m == 0) {
            return 0;
        }
        
        int res, max_res;
        
        max_res = 0;
        for (i = 0; i < n; ++i) {
            for (j = 0; j < m; ++j) {
                if (!v[i][j]) {
                    continue;
                }
                res = 0;
                
                v[i][j] = 0;
                ++res;
                DFS(v, i, j, res);
                max_res = res > max_res ? res : max_res;
            }
        }
        
        return max_res;
    };
private:
    int n, m;
    
    void DFS(vector<vector<int> > &v, int i, int j, int &res) {
        static const int d[4][2] = {
            {-1,  0}, 
            { 0, -1}, 
            {+1,  0}, 
            { 0, +1}
        };
        
        int k, x, y;
        for (k = 0; k < 4; ++k) {
            x = i + d[k][0];
            y = j + d[k][1];
            if (x < 0 || x > n - 1 || y < 0 || y > m - 1) {
                continue;
            }
            if (!v[x][y]) {
                continue;
            }
            v[x][y] = 0;
            ++res;
            DFS(v, x, y, res);
        }
    };
};