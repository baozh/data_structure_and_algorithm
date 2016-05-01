float power(float x, int y) {
    float temp;
    if (y< 0)
        return 1/power(x, -y);
    if( y == 0)
       return 1;
    temp = power(x, y>>1);
    if (y&1 == 0)
        return temp*temp;
    else
        return y>0? temp*temp*x: temp*temp/x;
}