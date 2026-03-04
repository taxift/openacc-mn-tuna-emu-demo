// 仕様上目標としている3次元ステンシルのコード
// [16][16][32]の格子で分割
#include <stdio.h>
#include <stdlib.h>
#define N 128
#define N_X N
#define N_Y N
#define N_Z N
#define ARRAY_SIZE :N_X][:N_Y][:N_Z

int main() {
    double array[N_X][N_Y][N_Z];
    double c1 = -1.5;

    for(int i=0; i<N_X; i++) {
        for(int j=0; j<N_Y; j++) {
            for(int k=0; k<N_Z; k++) {
                array[i][j][k] = 1.0f;
            }
        }
    }

// Host to PE（PE 配置はデフォルトでいいなら省略可）
    // 各次元の袖領域をshadow 節で指定[マイナスの要素数:プラスの要素数]
// #ifdef __MNACC__
// #define SHADOW shadow(array[1][1][1])
// #else
// #define SHADOW
// #endif
#pragma acc enter data copyin(array[ARRAY_SIZE]) //SHADOW
// #pragma acc enter data create(temp[0:N_X][0:N_Y][0:N_Z]) copyin(a[0:N_X][0:N_Y][0:N_Z]

// オフロード範囲の指定（parallel ディレクティブ）
    // ループ分割はpresent 節の最初に指定された配列に従う
    // 袖を持つ配列がある場合はそれを最初に指定
    // 分割コンパイル対応でshadow 節をここでも指定
#pragma acc parallel present(array[0:N_X][0:N_Y][0:N_Z]) shadow(array[0:1][0:1][0:1])
{
    for(int count=0; count<1000; count++) {

// tempが無いので更新後の値を見れないから、-方向は参照しない
#pragma acc loop gang collapse(3)
        for(int i=1; i<N_X-1; i++){
            for(int j=1; j<N_Y-1; j++){
                for(int k=1; k<N_Z-1; k++){
                    array[i][j][k] =
                        c1*(array[i+1][j][k]
                        +array[i][j+1][k]
                        +array[i][j][k+1]
                        +array[i][j][k]);
                }
            }
        }

// reflect で袖交換の発生をコンパイラに知らせる
#pragma acc reflect(array)

    } // count の for 文（＝オフロード適用範囲）の終了
}
#pragma acc exit data copyout(array[ARRAY_SIZE])
// #pragma acc exit data copyout(a[0:N_X][0:N_Y][0:N_Z]) delete(temp[0:N_X][0:N_Y][0:N_Z])
}

