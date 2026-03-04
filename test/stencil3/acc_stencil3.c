// [16][16][32]の格子で分割
#include <stdio.h>
#include <stdlib.h>
#define N 128
#define N_X N
#define N_Y N
#define N_Z N

int main() {
    double (*a)[N_Y][N_Z] = (double(*)[N_Y][N_Z])malloc(sizeof(double) * N_X * N_Y * N_Z);
    double c1 = -1.5;
    // double c2 = 1.5;

    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                a[i][j][k] = 1.0;
            }
        }
    }

    double (*temp)[N_Y][N_Z] = (double(*)[N_Y][N_Z])malloc(sizeof(double) * N_X * N_Y * N_Z);
    // double temp[N_X][N_Y][N_Z];

// Host to PE（PE 配置はデフォルトでいいなら省略可）
    // 各次元の袖領域をshadow 節で指定[マイナスの要素数:プラスの要素数]
#ifdef __MNACC__
#define SHADOW shadow(array1[1], array2[1])
#else
#define SHADOW
#endif
// #pragma acc enter data create(temp[:N_X][:N_Y][:N_Z]) copyin(a[:N_X][:N_Y][:N_Z])
#pragma acc enter data copyin(temp[:N_X][:N_Y][:N_Z], a[:N_X][:N_Y][:N_Z])

// オフロード範囲の指定（parallel ディレクティブ）
    // ループ分割はpresent 節の最初に指定された配列に従う
    // 袖を持つ配列がある場合はそれを最初に指定
    // 分割コンパイル対応でshadow 節をここでも指定
#pragma acc parallel present(a[:N_X][:N_Y][:N_Z], temp[:N_X][:N_Y][:N_Z]) shadow(a[1][1][1])
    for(int count=0; count<1000; count++) {
#pragma acc loop gang collapse(3)
        for(int i=1; i<128-1; i++){
            for(int j=1; j<128-1; j++){
                for(int k=1; k<128-1; k++){
                    temp[i][j][k] =
                        c1*(a[i-1][j][k]+a[i+1][j][k]
                        +a[i][j-1][k]+a[i][j+1][k]
                        +a[i][j][k-1]+a[i][j][k+1]
                        +a[i][j][k]);
                }
            }
        }
#pragma acc loop gang collapse(3)
        for(int i=1; i<128-1; i++){
            for(int j=1; j<128-1; j++){
                for(int k=1; k<128-1; k++){
                    a[i][j][k] = temp[i][j][k];
                }
            }
        }
// reflect で袖交換の発生をコンパイラに知らせる
#pragma acc reflect(a)

    } // count の for 文（＝オフロード適用範囲）の終了

// #pragma acc exit data copyout(a[:N_X][:N_Y][:N_Z]) delete(temp[:N_X][:N_Y][:N_Z])
#pragma acc exit data copyout(a[:N_X][:N_Y][:N_Z])
}

