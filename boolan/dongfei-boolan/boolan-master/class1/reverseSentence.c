bool isPunctuationOrSpace(char *character) {
    return *character == ' ' || *character == ',' || *character == '.';
}

bool isNumber(char *character) {
    return *character >= '0' && *character <= '9';
}

bool needReverse(char *sentence, int *offset) {
    int length = (int)strlen(sentence);
    bool needReverse = false;
    *offset = 0;
    while (!isPunctuationOrSpace(sentence + (*offset)) && (*offset) < length) {
        if (!isNumber(sentence + (*offset))) {
            needReverse = true;
        }
        (*offset)++;
    }
    return needReverse;
}

void reverseWord(char *word, int length) {
    int i = 0, j = length - 1;
    while (i < j) {
        swap(*(word + i), *(word + j));
        i++;
        j--;
    }
}

void reverseSentence(char *sentence) {
    int length = (int)strlen(sentence);
    int offset;
    for (int i = 0; i < length;) {
        if (needReverse(sentence + i, &offset)) {
            reverseWord(sentence + i, offset);
        }
        i += (offset + 1);
    }
}
