//计算第n个素数
//方法：找出所有的前n个素数
//判断是否是素数：能否被之前所有的素数整除
int GetNthPrime( int n) {
    list<int> primes (1, 2);    // init list: length 1, value 2 (first prime)
    int number = 3;
    while(primes.size() != n) {
        bool isPrime = true;
        //从第一个素数作为起始值 来找 素数
        for(auto it = primes.begin(); it != primes.end() && (*it)*(*it) <= number; it++) {
            if(number % (*it) == 0)
                isPrime = false;
        }

        if (isPrime) {
            primes.push_back(number);
        }
        number += 2;    // even numbers greater than 2 are not prime
    }
    return *(primes.rbegin());
}
