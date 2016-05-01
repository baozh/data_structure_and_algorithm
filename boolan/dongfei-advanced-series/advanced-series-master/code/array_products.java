// Given an array of numbers, return array of products of all other numbers (no division)

int products[N]; // This is the result
int[] arrayProduct(int a[], int N) {
    int products_below[N];
    p=1;
    for(int i=0;i<N;++i) {
        products_below[i]=p;
        p*=a[i];
    }

    int products_above[N];
    p=1;
    for(int i=N-1;i>=0;--i) {
        products_above[i]=p;
        p*=a[i];
    }

    for(int i=0;i<N;++i) {
        products[i]=products_below[i]*products_above[i];
    }
    return products;
}

// O(1) in space
int[] arrayProduct(int a[], int N) {
    // Get the products below the current index
    p=1;
    for(int i=0;i<N;++i) {
        products[i]=p;
        p*=a[i];
    }

    // Get the products above the curent index
    p=1;
    for(int i=N-1;i>=0;--i) {
        products[i]*=p;
        p*=a[i];
    }
}
