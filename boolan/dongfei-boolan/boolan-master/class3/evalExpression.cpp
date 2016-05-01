int evalPostfix(string input) {
    int operand1, operand2, result;
    stack<int> operation;

    int i = 0;
    while (i < input.length()) 
    {
        if (isdigit(input[i])) 
        {
          operation.push(input[i]-'0');
        } 
        else 
        {
          operand2 = operation.top();
          operation.pop();
          operand1 = operation.top();
          operation.pop();
          switch (input[i]) 
          {
                  case '+': result = operand1 + operand2;
                    operation.push(result);
                    break;
                  case '-': result = operand1 - operand2;
                    operation.push(result);
                    break;
                  case '*': result = operand1 * operand2;
                    operation.push(result);
                    break;
                  case '/': result = operand1 / operand2;
                    operation.push(result);
                    break;
          }
        }
        i++;
    }

    return operation.top();
}
