// [16][16][32]の格子で分割
#include <stdio.h>
#include <stdlib.h>
#define N_X 128 //32
#define N_Y 128 //32
#define N_Z 128 //32
#define ARRAY_SIZE :N_X][:N_Y][:N_Z

int main() {
    double array1[N_X][N_Y][N_Z], array2[N_X][N_Y][N_Z];
    double (*array3)[N_Y][N_Z] = (double(*)[N_Y][N_Z])malloc(sizeof(double) * N_X * N_Y * N_Z);

    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                array1[i][j][k] = 1.0f;
                array2[i][j][k] = 1.0f;
                array3[i][j][k] = 1.0f;
            }
        }
    }
int a = 1;

#ifdef __MNACC__
#define SHADOW shadow(array1[a], array2[1])
#else
#define SHADOW
#endif
#pragma acc enter data copyin(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE]) SHADOW

#pragma acc parallel loop gang collapse(3) shadow(array1[1:1][1:1][1], array2[1:1][1:1][1], array3[1:1][1:1][1]) present(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE])
    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                array1[i][j][k] += a* array2[i][j][k] * array3[i][j][k];
                
                float b = 2.0;
#pragma acc loop vector
                for(int l=0; l<4; l++)
                    array1[i][j][k] *= b;

#pragma acc reflect(array1, array2, array3)

            }
        }
    }

#pragma acc exit data copyout(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE])

}

