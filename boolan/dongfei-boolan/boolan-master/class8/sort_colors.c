// 2 pass, Counting sort solution
void sortColors(int A[], int n) {   
    int red=0, white =0, blue=0;   
    for (int i =0; i < n; i++) {   
         switch (A[i]) {
         case 0:
               red++;break;
          case 1:
               white++;break;   
          case 2:
               blue++;break;   
          }   
     }   

     for (int i =0; i<n; i++) {   
          if (red>0) {   
               A[i]=0;   
               red--;   
               continue;   
          }   
          if (white>0) {   
               A[i] =1;   
               white--;   
               continue;   
          }   
          A[i]=2;   
     }   
}   

// one pass, two pointers solution
void sortColors(int A[], int n) {   
     int redSt=0, blueSt=n-1;   
     int i=0;   
     while (i < blueSt+1) {   
          if (A[i]== 0) {   
                std::swap(A[i], A[redSt]);   
                redSt++;   
                i++;   
                continue;   
           }   
           if (A[i] == 2) {   
                std::swap(A[i], A[blueSt]);   
                blueSt--;
                continue;   
           }   
           i++;  
      }   
 } 
