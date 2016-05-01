class Solution {
public:
    bool checkPowerOf2(int n) {
        if (1 > n) {
            return false;
        } else {
            return 0 == (n & (n - 1));
        }
    }
};
