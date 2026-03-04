// [16][16][32]の格子で分割
#include <stdio.h>
#include <stdlib.h>
#define N_X 32
#define N_Y 32
#define N_Z 32
#define ARRAY_SIZE :N_X][:N_Y][:N_Z

int main() {
    float array1[N_X][N_Y][N_Z], array2[N_X][N_Y][N_Z], ***array3;
    array3 = (float***) malloc(sizeof(float**) * N_X);
    array3[0] = (float**) malloc(sizeof(float*) * N_X * N_Y);
    array3[0][0] = (float*) malloc(sizeof(float) * N_X * N_Y * N_Z);
    for(int i=0; i<N_X; i++) {
        array3[i] = array3[0] + i*N_Y;
        for(int j=0; j<N_Y; j++) {
            array3[i][j] = array3[0][0] + i*N_Y*N_Z + j*N_Z;
        }
    }

    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                array1[i][j][k] = 1.0f;
                array2[i][j][k] = 1.0f;
                array3[i][j][k] = 1.0f;
            }
        }
    }

#ifdef __MNACC__
#pragma acc enter data copyin(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE])
#endif

#pragma acc parallel loop gang collapse(3) present(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE])
    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                array1[i][j][k] += array2[i][j][k] * array3[i][j][k];
                
                float b = 2.0;
#pragma acc loop vector
                for(int l=0; l<4; l++)
                    array1[i][j][k] *= b;
            }
        }
    }

#pragma acc exit data copyout(array1[ARRAY_SIZE], array2[ARRAY_SIZE], array3[ARRAY_SIZE])

}

