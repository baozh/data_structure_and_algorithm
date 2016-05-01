import java.io.*;
import java.util.*;

/**
Write a function/method that takes a mathematical expression represented as a string and returns the value of the expression.

The expression is written in infix notation (number operator number) with a single space between operators and operands.

You only need to support four operations: addition, subtraction, division and multiplication (+ - * /)

No parenthesis.

Valid input:
"15 + 2 - 3" -> 14
"145" -> 145
"1 + 233 / 233" -> 2


Invalid input:
"1 1"
"+ 1"
"1 + 2 +"
"+ 1 1"
"1+2"

Solution 
1) Convert to Postfix
  15 2 + 3 - 
  1 233 233 / +
2) Evaluate Postfix

map<Stirng, Int>:

+: 1
-: 1
*: 2
/: 2
*/

class Solution {
    Map<String, Integer> map = new HashMap<String, Integer>();

    public Solution() {
        map = new HashMap<String, Integer>();
        map.put("+", 1);
        map.put("+", 1);
        map.put("*", 2);
        map.put("/", 2);
    }

  boolean operatorGreaterThan(String op1, String op2) {
    return map.get(op1) >= map.get(op2);
  }

  List<String> convert2Postfix(String infix) {
    String[] values = infix.split(" ");
    Stack<String > stack = new Stack<String>();
    List<String> out = new ArrayList<String>();
    for (String v: values) {
      if (isOperator(v)) {
        while (!stack.isEmpty()) {
          if (operatorGreaterThan(stack.peek(), v)) {
            out.add(stack.pop());
          } else
            break;
        }
        stack.push(v);
      } else if (isOperand(v)) {
        out.add(v);
      }
    }

    while (!stack.empty())
      out.add(stack.pop());
    return out;
  }

  boolean isOperator(String s) {
    if (s.equals("+") || s.equals("-") || s.equals("*") || s.equals("/"))
      return true;
    else
      return false;
  }

  boolean isOperand(String s) {
    char[] nums = s.toCharArray();
    for (int i=0; i<nums.length; i++) {
      if (nums[i] < '0' || nums[i] > '9')
        return false;

    }
    return true;
  }

  int evalutePostfix(List<String> postfix) {
    Stack<Integer> stack = new Stack<Integer>();

    for (String s: postfix) {
      if (isOperand(s))
        stack.push(Integer.parseInt(s));
      else if (isOperator(s)) {
        int op1 = stack.pop();
        int op2 = stack.pop();
        int result;

        switch(s) {
          case "+": result = op2 + op1;
            stack.push(result);
            break;
          case "-": result = op2 - op1;
            stack.push(result);
            break;
          case "*": result = op2 * op1;
            stack.push(result);
            break;
          case "/": result = op2 / op1;
            stack.push(result);
            break;
        }
      }
    }
    return stack.pop();

  }

  int evalute(String s) {
    return evalutePostfix(convert2Postfix(s));
  }

  public static void main(String[] args) {
    Solution s = new Solution();
    System.out.println(s.evalute("2 * 3 + 1 + 4 / 4"));
  }
}