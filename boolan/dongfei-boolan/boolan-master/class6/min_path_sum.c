class Solution {
public:
    int minPathSum(vector<vector<int> > &grid) {
        if(grid.empty() || grid[0].empty()) {
            return 0;
        }

        int row = grid.size();
        int col = grid[0].size();

        int dp[row][col];

        dp[0][0] = grid[0][0];
        for(int i = 1; i < row; i++) {
            dp[i][0] = dp[i - 1][0] + grid[i][0];
        }

        for(int j = 1; j < col; j++) {
            dp[0][j] = dp[0][j - 1] + grid[0][j];
        }

        for(int i = 1; i < row; i++) {
            for(int j = 1; j < col; j++) {
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }

        return dp[row - 1][col - 1];
    }
};
