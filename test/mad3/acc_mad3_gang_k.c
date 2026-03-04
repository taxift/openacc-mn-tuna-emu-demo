// 1次元配列が8192以下で、2次元配列が存在する（各PEで計算させる）場合
#include <stdio.h>
// #define N 8192
#define N 16384

int main() {
    float array1[100][100][N], array2[100][100][N], ***array3;
    array3 = (float***) malloc(sizeof(float**) * N);
    array3[0] = (float**) malloc(sizeof(float*) * N * 100);
    array3[0][0] = (float*) malloc(sizeof(float) * N * 100 * 100);
    for(int i=0; i<N; i++) {
        array3[i] = array3[0] + i*100;
        for(int j=0; j<100; j++) {
            array3[i][j] = array3[0][0] + i*100*100 + j*100;
        }
    }

            for(int k=0; k<N; k++) {
        for(int j=0; j<100; j++) {
    for(int i=0; i<100; i++) {
                array1[i][j][k] = 1.0f;
                array2[i][j][k] = 1.0f;
                array3[i][j][k] = 1.0f;
            }
        }
    }

#pragma acc enter data copyin(array1[:100][:100][:N], array2[:100][:100][:N], array3[:100][:100][:N])

#pragma acc parallel loop gang present(array1[:100][:100][:N], array2[:100][:100][:N], array3[:100][:100][:N])
            for(int k=0; k<N; k++) {
        for(int j=0; j<100; j++) {
    for(int i=0; i<100; i++) {
                array1[i][j][k] += array2[i][j][k] * array3[i][j][k];
                
                float b = 2.0;
#pragma acc loop vector
                for(int l=0; l<4; l++)
                    array1[i][j][k] *= b;
            }
        }
    }

#pragma acc exit data copyout(array1[:100][:100][:N], array2[:100][:100][:N], array3[:100][:100][:N])

}

