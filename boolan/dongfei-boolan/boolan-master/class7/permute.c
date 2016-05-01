//全排列
void Permutation(char* pStr, char* pBegin) {
    assert(pStr && pBegin);

    if(*pBegin == '\0')
        printf("%s\n",pStr);
    else {
        for(char* pCh = pBegin; *pCh != '\0'; pCh++) {
            swap(*pBegin,*pCh);
            Permutation(pStr, pBegin+1);
            swap(*pBegin,*pCh);
        }
    }
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

//从size个字符中选取m个字符进行全排列，打印排列结果
void Permutation(char str[], int index, int m, int size) {
    if (index == m) {  //和本文第一个程序比只是这里打印时不太相同
        for (int i = 0; i < m; i++)
            cout << str[i];
        cout << "\t";
    }
    else {
        for (int i = index; i < size; i++) {
            swap(str[index], str[i]);
            Permutation(str, index + 1, m, size);
            swap(str[index], str[i]);
        }
    }
}

void Permutation(char str[], int m, int size) {
    Permutation(str, 0, m, size);
}