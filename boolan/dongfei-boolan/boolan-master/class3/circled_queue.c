class Queue {
    public:
        Queue() {
            _front = 0;
            _rear = 0;
        }
        int push(int data);
        int pop();
        bool isEmpty();
        bool isFull();
        ~Queue() {}
    private:
        static const int _size = 10;
        int _data[_size];
        int _front;
        int _rear;
};

int Queue::push(int data){
    if (!isFull()) {
        _rear = (_rear + 1) % _size;
        _data[_rear] = data;
        return 0;
    } else {
        return -1;
    }
}

int Queue::pop() {
    if (!isEmpty()) {
          _front = (_front + 1) % _size;
          return _data[_front];
    } else {
        return -1;
    }
}

bool Queue::isEmpty() {
    if (_front == _rear)
        return 1;
    else
        return 0;

}

bool Queue::isFull() {
    if ((_rear+1) % _size == _front)
        return 1;
    else
        return 0;
}
