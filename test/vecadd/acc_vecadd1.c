// 鈴木さんへ渡したのはこれ
#include <stdio.h>
// #define N 4096
#define N 8192
// #define N 16384

int  main() {
    double array1[N], array2[N], array3[N];
    
    for(int j=0; j<N; j++) {
        array1[j] = 0.0;
        array2[j] = 1.0;
        array3[j] = 1.0;
    }

#pragma acc enter data copyin(array1[:N], array2[:N], array3[:N])
// #pragma acc enter data create(array1[:N]) copyin(array2[:N], array3[:N])

#pragma acc parallel loop gang present(array1[:N], array2[:N], array3[:N])
    for(int i=0; i<N; i++) {
        array1[i] = array2[i] + array3[i];
    }

#pragma acc exit data copyout(array1[:N], array2[:N], array3[:N])
// #pragma acc exit data copyout(array1[:N]) delete(array2[:N], array3[:N])

}

