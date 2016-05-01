bool isUnique(string input) {
    bitset<256> hashMap;
    for (int i = 0; i < input.length(); i++) {
        if (hashMap[(int)input[i]]) {
            return false;
        }
        hashMap[(int)input[i]] = 1;
    }
    
    return true;
}