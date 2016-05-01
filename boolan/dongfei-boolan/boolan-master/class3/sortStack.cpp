stack<int> sort(stack<int> &input) 
{
    stack<int> output;
    while (!input.empty()) 
    {
        int value = input.top();
        input.pop();
        //以递减的顺序 将元素插入到output中
        while (!output.empty() && output.top() < value) 
        {
        //把当前比value大的元素，都 插入到input中，之后 再进行比较后，会再按顺序插入到output中.
            input.push(output.top());
            output.pop();
        }
        output.push(value);
    }
    return output;
}
