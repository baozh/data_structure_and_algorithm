struct cmp {
    bool operator() (const int &a, const int &b) {
        if (a > b)
            return false;
        else
            return true;
    }
};

const int NO_ERROR = 0;
const int ERROR = 1;

ErrorCode kthLargest(priority_queue<int, vector<int>, cmp> &heap, int &data, int k, int &outValue) {
    if (heap.size() < k) 
    {
        heap.push(data);
        if(heap.size() == k) 
        {
            outValue = heap.top();
            return NO_ERROR;
        }
        // not enough data
        return ERROR;
    }

    if (heap.top() < data) 
    {
        heap.pop();
        heap.push(data);
    }

    outValue = heap.top();
    return NO_ERROR;
}

/*
void main() {
    int outValue;
    priority_queue<int, vector<int>, cmp> heap;
    while (cin >> data) {
        if (kthLargest(heap, data, k, outValue) == NO_ERROR)
            cout << outValue; 
    }
}
*/
