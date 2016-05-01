#define INT_BIT_LENGTH (32)
void setBits(unsigned int &N, unsigned int M, int i, int j) {
    unsigned int max = ~0;
    unsigned int bitMask = (max << (INT_BIT_LENGTH - i) |
                            max >> j); //11..100..011..1
    N = (M & (~bitMask)) | (N & bitMask);
}
