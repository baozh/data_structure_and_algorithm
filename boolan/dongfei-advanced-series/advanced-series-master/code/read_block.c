/*
Round 4 was again a pure coding question.  All you need is to think carefully about all situations and code it bug-free.  But the problem itself is kind of convoluted; it took me a lot of time to clarify what the interviewer meant. After the clarification, the time is limited and I also barely finished it.
The problem is like this: you have an incoming byte streams with unknown size.  You have a function int read_block(stream* s), which can read a fixed number of bytes from the stream at a time (say, 1024). However, the stream may end before it reads that many.  So, this function returns the number of bytes that it actually reads. Now, given this function, write a class that reads arbitrary number of bytes, also return the number of bytes that it actually reads.  One shall be able to use this function repeatedly, i.e., for each read, it should begin from the point where the previous read ends.
Say, you have a stream of 3,000 bytes, and read_block() read 1024 at a time.
*/
int window = 1024;
byte[] get(int size) {
    static int left = 0;
    byte[] buffer = new byte[window];
    byte[] result = new byte[size];
    if (left == 0 ) {
        read_block(buffer);
        left = window;
    }

    if (left > size) {
        memcpy(result, buffer + (window - left), size);
        left = left - size;
    } else {
        memcpy(result, buffer + (window - left), left);
        int need = size - left;
        byte* p = result + left;
        while ( need > window) {
            read_block(buffer);
            memcpy(p, buffer, window);
            p += window;
            need -= window;
        }
        read_block(buffer);
        memcpy(p, buffer, need);
        left = window - need;
    }
    return result;
}
