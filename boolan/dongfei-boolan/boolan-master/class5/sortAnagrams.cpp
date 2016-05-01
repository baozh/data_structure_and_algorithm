#define HASH_TAG_SIZE (127)
struct StringWithTag {
    string value;
    int tag[HASH_TAG_SIZE];
};

void hashString(StringWithTag &aStringWithTag) {
    memset(aStringWithTag.tag, 0, sizeof(int) * HASH_TAG_SIZE);
    for (int i = 0; i < aStringWithTag.value.size(); i++) {
        aStringWithTag.tag[aStringWithTag.value[i]]++;
    }
}

void myRadixSort(vector<StringWithTag> &arrayWithTag, int digit, unsigned long radix) {
    // implement a RadixSort
}

void sortAnagrams(vector<string> &array) {
    vector<StringWithTag> arrayWithTag;
    unsigned long radix = 0;
    for (auto i = array.begin(); i != array.end(); i++) {
        StringWithTag aStringWithTag;
        aStringWithTag.value = *i;
        if ((*i).length() > radix) {
            radix = (*i).length();
        }
        hashString(aStringWithTag);
        arrayWithTag.push_back(aStringWithTag);
    }

    myRadixSort(arrayWithTag, HASH_TAG_SIZE, radix);

    for (int i = 0; i < arrayWithTag.size(); i++) {
        array[i] = arrayWithTag[i].value;
    }
}
