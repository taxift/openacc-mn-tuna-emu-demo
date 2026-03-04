// 1次元目が8192以下の2次元配列が存在する場合
// （エレメントワイズなので1次元だけ分割して2次元目以降を各PEで計算させる）
#include <stdio.h>
#define N 8192

int  main() {
    float array1[N][100], array2[N][100], **array3;
    array3 = (float**) malloc(sizeof(float*) * N);
    array3[0] = (float*) malloc(sizeof(float) * N * 100);
    for(int i=0; i<N; i++) {
        array3[i] = array3[0] + i*100;
    }

    for(int i=0; i<N; i++) {
        for(int j=0; j<100; j++) {
            array1[i][j] = 1.0f;
            array2[i][j] = 1.0f;
            array3[i][j] = 1.0f;
        }
    }

#pragma acc enter data copyin(array1[:N][:100], array2[:N][:100], array3[:N][:100])

#pragma acc parallel loop gang present(array1[:N][:100], array2[:N][:100], array3[:N][:100])
    for(int i=0; i<N; i++) {
        for(int j=0; j<100; j++) {
            array1[i][j] += array2[i][j] * array3[i][j];

            float b = 2.0;
#pragma acc loop vector
            for(int l=0; l<4; l++)
                array1[i][j] *= b;
        }
    }

#pragma acc exit data copyout(array1[:N][:100], array2[:N][:100], array3[:N][:100])

}

