//将字符串abcde12345 inplace的转换为  a1b2c3d4e5
//inplace 的意思就是O(1)的空间
int clacTarget(int nLen, int nIndex) {
        assert(nLen > 0 && nIndex > 0 && nLen > nIndex);
        if (nIndex < nLen/2)
                return 2 * nIndex;

        return (nIndex - nLen/2) * 2 + 1;
}

char* inplaceConvert(char* str) {
        if (NULL == str)
                return NULL;

        int nLen = strlen(str);
        if (0 == nLen || nLen%2 != 0)
                return str;

        int nCount = nLen - 2;
        for (int i = 1; i < nLen && nCount > 0; i += 2) {
                int nCur = clacTarget(nLen, i);
                char cTmp = str[nCur];
                str[nCur] = str[i];
                nCount--;

                while (nCur != i) {
                        nCur = clacTarget(nLen, nCur);
                        char c = str[nCur];
                        str[nCur] = cTmp;
                        cTmp = c;
                        nCount--;
                }
        }
        return str;
}