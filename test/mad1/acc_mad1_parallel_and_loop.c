#include <stdio.h>
#define N 8192

int  main() {
    float array1[N], array2[N], *array3;
    array3 = (float*) malloc(sizeof(float) * N );

    for(int j=0; j<N; j++) {
        array1[j] = 1.0f;
        array2[j] = 1.0f;
        array3[j] = 1.0f;
    }

#pragma acc enter data copyin(array1[:N], array2[:N], array3[:N])

#pragma acc parallel
#pragma acc loop gang
    for(int i=0; i<N; i++) {
        array1[i] += array2[i] * array3[i];

        float b = 2.0;
#pragma acc loop vector
        for(int l=0; l<4; l++)
            array1[i] *= b;
    }

#pragma acc exit data copyout(array1[:N], array2[:N], array3[:N])

}
