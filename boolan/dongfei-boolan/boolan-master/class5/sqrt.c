const int ERROR = -1;
const double precision = 0.00001;
double mySqrtHelper(double x, double lowBound, double highBound) {
    double sqrt = lowBound / 2 + highBound / 2;
    if (abs(sqrt * sqrt - x) <  precision) {
        return sqrt;
    } else if (sqrt * sqrt - x > 0) {
        return mySqrtHelper(x, lowBound, sqrt);
    } else {
        return mySqrtHelper(x, sqrt, highBound);
    }
}

double mySqrt(double x) {
    if (x < 0) {
        return ERROR;
    }
        
    if (x == 0) {
        return 0;
    }

    if (x == 1) {
        return 1;
    }

    if (x < 1) {
        return mySqrtHelper(x, x, 1);
    } else {
        return mySqrtHelper(x, 1, x);
    }
}

// Newton method
float squareRoot(float n) {
  /*We are using n itself as initial approximation
   This can definitely be improved */
  float x = n;
  float y = 1;
  float e = 0.000001; /* e decides the accuracy level*/
  while(x - y > e) {
    x = (x + y)/2;
    y = n/x;
  }
  return x;
}
