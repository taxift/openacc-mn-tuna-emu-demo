// 1次元目が8192以下の2次元配列が存在する場合
// （エレメントワイズなので1次元だけ分割して2次元目以降を各PEで計算させる）
#include <stdio.h>
#define N_X 128
#define N_Y 128

int  main() {
    float array1[N_X][N_Y], array2[N_X][N_Y], **array3;
    array3 = (float**) malloc(sizeof(float*) * N_X);
    array3[0] = (float*) malloc(sizeof(float) * N_X * N_Y);
    for(int i=0; i<N_X; i++) {
        array3[i] = array3[0] + i*N_Y;
    }

    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            array1[i][j] = 1.0f;
            array2[i][j] = 1.0f;
            array3[i][j] = 1.0f;
        }
    }

#pragma acc enter data copyin(array1[:N_X][:N_Y], array2[:N_X][:N_Y], array3[:N_X][:N_Y])

#pragma acc parallel loop gang collapse(2) present(array1[:N_X][:N_Y], array2[:N_X][:N_Y], array3[:N_X][:N_Y])
    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            array1[i][j] += array2[i][j] * array3[i][j];

            float b = 2.0;
#pragma acc loop vector
            for(int l=0; l<4; l++)
                array1[i][j] *= b;
        }
    }

#pragma acc exit data copyout(array1[:N_X][:N_Y], array2[:N_X][:N_Y], array3[:N_X][:N_Y])

}

