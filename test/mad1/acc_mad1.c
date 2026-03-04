#include <stdio.h>
#define N 8192
#define M 8191
// #define L 8193
#define K 16384  // 8192*2

int  main() {
    float array1[N], array2[N], *array3;
    array3 = (float*) malloc(sizeof(float) * N );

    for(int j=0; j<N; j++) {
        array1[j] = 1.0f;
        array2[j] = 1.0f;
        array3[j] = 1.0f;
    }

#pragma acc enter data copyin(array1[:N], array2[:N], array3[:N])

#pragma acc parallel loop gang present(array1[:N], array2[:N], array3[:N])
    for(int i=0; i<N; i++) {
        array1[i] += array2[i] * array3[i];

        int array4[4];
        float b = 2.0;
#pragma acc loop vector
        for(int l=0; l<4; l++)
            array4[i] *= b;
    }

#pragma acc parallel loop gang present(array1[:M], array2[:M], array3[:M])
    for(int i=0; i<M; i++) {
        array1[i] += array2[i] * array3[i];

        int array4[4];
        float b = 2.0;
#pragma acc loop vector
        for(int l=0; l<4; l++)
            array4[i] *= b;
    }

// #pragma acc parallel loop gang present(array1[:L], array2[:L], array3[:L])
//     for(int i=0; i<L; i++) {
//         array1[i] += array2[i] * array3[i];

//         int array4[4];
//         float b = 2.0;
// #pragma acc loop vector
//         for(int l=0; l<4; l++)
//             array4[i] *= b;
//     }

#pragma acc parallel loop gang present(array1[:K], array2[:K], array3[:K])
    for(int i=0; i<K; i++) {
        array1[i] += array2[i] * array3[i];

        int array4[4];
        float b = 2.0;
#pragma acc loop vector
        for(int l=0; l<4; l++)
            array4[i] *= b;
    }

#pragma acc exit data copyout(array1[:N], array2[:N], array3[:N])

}

