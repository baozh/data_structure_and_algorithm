int numberOfDifferentBits(int A, int B) {
    int diff = A ^ B;
    int count = 0;
    while (diff > 0) {
        count += diff & (0x1);
        diff = diff >> 1;
/* or do the following:
        diff &= (diff – 1); // clear lowest 1 in diff
        count++;
*/
    }
    return count;
}
