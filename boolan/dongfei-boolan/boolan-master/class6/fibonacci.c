// recursion
int Fibonacci(int n) {
    if(n == 0)
        return 0;
    if(n == 1)
        return 1;
    return Fibonacci(n-1) + Fibonacci(n-2);
}
   
// DP
int fibonacci(int n) {
  int a = 0;
  int b = 1;
  int sum;
  int i;

  if(n == 0)
    return 0;
  if(n == 1) 
    return 1;
  /**
   * Here is the standard for loop. This will step through, performing the code
   * inside the braces until i is equal to n.
   */
  for (i = 0; i < n-1; i++) {
    sum = a + b;
    a = b;
    b = sum;
  }
  return sum;
}

// Multiply two matrices
// Input: matrix1 - the first matrix
//        matrix2 - the second matrix
//Output: the production of two matrices
///////////////////////////////////////////////////////////////////////
Matrix2By2 MatrixMultiply
(
      const Matrix2By2& matrix1, 
      const Matrix2By2& matrix2
)
{
      return Matrix2By2(
            matrix1.m_00 * matrix2.m_00 + matrix1.m_01 * matrix2.m_10,
            matrix1.m_00 * matrix2.m_01 + matrix1.m_01 * matrix2.m_11,
            matrix1.m_10 * matrix2.m_00 + matrix1.m_11 * matrix2.m_10,
            matrix1.m_10 * matrix2.m_01 + matrix1.m_11 * matrix2.m_11);
}

///////////////////////////////////////////////////////////////////////
// The nth power of matrix
 // 1  1
//  1  0
///////////////////////////////////////////////////////////////////////
Matrix2By2 MatrixPower(unsigned int n)
{
  assert(n > 0);

  Matrix2By2 matrix;
  if(n == 1)
  {
    matrix = Matrix2By2(1, 1, 1, 0);
  }
  else if(n % 2 == 0)
  {
    matrix = MatrixPower(n / 2);
    matrix = MatrixMultiply(matrix, matrix);
  }
  else if(n % 2 == 1)
  {
    matrix = MatrixPower((n - 1) / 2);
    matrix = MatrixMultiply(matrix, matrix);
    matrix = MatrixMultiply(matrix, Matrix2By2(1, 1, 1, 0));
  }

  return matrix;
}

// templates
struct Fib
{
    enum{
        val = Fib<N-1>::val + Fib<N-2>::val
    };
};
template <>
struct Fib<0>
{
    enum{
        val = 0
    };
};

template <>
struct Fib<1>
{
    enum{
        val=1
    };
};
