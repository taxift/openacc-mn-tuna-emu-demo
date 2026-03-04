// MPIも併用する前提で、128に左右の袖１つずつくっついて130^3サイズ
// [16][16][32]の格子で分割
#include <stdio.h>
#include <stdlib.h>
#define N 130
#define N_X N
#define N_Y N
#define N_Z N
#define ARRAY_SIZE :N_X][:N_Y][:N_Z

int main() {
    double (*a)[N_Y][N_Z] = (double(*)[N_Y][N_Z])malloc(sizeof(double) * N_X * N_Y * N_Z);
    double c1 = -1.5;
    double c2 = 1.5;

    for(int i=1; i<N_X-1; i++) {
        for(int j=1; j<N_Y-1; j++) {
            for(int k=1; k<N_Z-1; k++) {
                a[i][j][k] = 1.0f;
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
// #pragma acc enter data create(temp[1:128][1:128][1:128]) copyin(a[1:128][1:128][1:128])
#pragma acc enter data copyin(temp[1:128][1:128][1:128], a[1:128][1:128][1:128])

// オフロード範囲の指定（parallel ディレクティブ）
    // ループ分割はpresent 節の最初に指定された配列に従う
    // 袖を持つ配列がある場合はそれを最初に指定
    // 分割コンパイル対応でshadow 節をここでも指定
// OpenACCとしては、あくまでOpenACC内のループで計算するところを投げる（MPIの袖は含めない）
    // 端のPEの袖のうち他のPEと接していない袖をMPIの袖とすることは、acc updateで指示する
        // もしくは「MPIの袖がa[1:1]で外側にあります」という指示をできたほうが良いかも？（周期境界のときと同じく別オプションにする）
#pragma acc parallel present(a[1:128][1:128][1:128], temp[1:128][1:128][1:128]) shadow(a[1:1][1:1][1:1]) 
    for(int count=0; count<1000; count++) {
#pragma acc loop gang collapse(3)
        for(int i=1; i<129; i++){
            for(int j=1; j<129; j++){
                for(int k=1; k<129; k++){
                    temp[i][j][k] =
                        c1*(a[i-1][j][k]+a[i+1][j][k]
                        +a[i][j-1][k]+a[i][j+1][k]
                        +a[i][j][k-1]+a[i][j][k+1])
                        +c2*a[i][j][k];
                }
            }
        }
#pragma acc loop gang collapse(3)
        for(int i=1; i<129; i++){
            for(int j=1; j<129; j++){
                for(int k=1; k<129; k++){
                    a[i][j][k] = temp[i][j][k];
                }
            }
        }
// reflect で袖交換の発生をコンパイラに知らせる
#pragma acc reflect(a)

// 
// MPIの前にデバイスの袖をホストに持ってくる必要があるので、
// 周期境界と違ってreflectじゃなくてupdateディレクティブの拡張とする
#pragma acc update host_bound(a[1:1][1:1][1:1])
// ここにMPIの袖交換の処理が入る
    // 交換するのは表面の6面
    // a[0][0~129][0~129], a[129][0~129][0~129],
    // a[0~129][0][0~129], a[0~129][129][0~129],
    // a[0~129][0~129][0], a[0~129][0~129][129]
    // OpenACCではacc updateが必要
    // TODO: ただし、いちいち上記のような範囲をOpenACCで指定するのは怠いので、
    // 「境界（PEの端）の袖はこのデータ（MPIの袖が入っている配列）でアップデートします」という指示ができると画期的かも？
    // （まあMPIで同じように書くなら、愚直に書くのもそんなに苦じゃないかもしれないけど）
    // このデータで、は別に配列名はホストとデバイスで一緒なので省略させる
#pragma acc update device_bound(a[1:1][1:1][1:1])
// 

    } // count の for 文（＝オフロード適用範囲）の終了

// #pragma acc exit data copyout(a[1:128][1:128][1:128]) delete(temp[1:128][1:128][1:128])
#pragma acc exit data copyout(a[1:128][1:128][1:128], temp[1:128][1:128][1:128])
}

