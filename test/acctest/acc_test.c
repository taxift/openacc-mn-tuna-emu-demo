#include <stdio.h>
#include <stdlib.h>
#define N 100

float* ptr;// = 1;
struct hoge{
    float uwaaaa;
};
struct hoge fuga;
struct hoge* fugafuga = &fuga;
void kansuu(struct hoge* a);
typedef float float_new;
union yuni{};

int main() {
    
    const float array2[N];
    float array1[N], *array3, sum;
    float_new array4[N];
    float* p_array[N];
    int i,k,l;
    const float teisuu = 3.41;
    float const uhyooo = 4.0;
    float* ptrconst0;
    const float* ptrconst1;       // ポインタアドレスが定数化（中身は書き換え可能）
    float const * ptrconst2;      // 同上
    float* const ptrconst3;       // 中身が定数化（ポインタアドレスは書き換え可能）
    const float* const ptrconst4; // 中身もポインタアドレスも定数化

struct hoge{
    float uwaaaa;
};
struct hoge fuga;
struct hoge* fugafuga = &fuga;
// void kansuu(struct hoge* a);
typedef float float_new;

    array3 = (float*) malloc(sizeof(float) * N );

    for(int j=0; j<N; j++) {
        array1[j] = 1.0f;
        // array2[j] = 1.0f;
        array3[j] = 1.0f;
        array4[j] = 1.0f;
    }

#pragma acc enter data copyin(array1[:N]) copyin(array2[:N], array3[:N], array4[:N])

// #pragma acc data copy(array1[:N])
// {
#pragma acc parallel loop present(array1[:N], array2[:N], array3[:N], array4[:N], ptrconst0[:1], ptrconst1[:1], ptrconst2[:1], ptrconst3[:1], ptrconst4[:1]) \
    gang vector(4)
// #pragma acc kernels
{
// int c=0;
// int array_local[N];
// int* ptr_local;
// #pragma acc loop
// #pragma omp parallel for
    for( i=0; i<N; i++) {
        float array5[N];
        array1[i] = (float)((int)(array1[i] * array2[i]) % (int)array3[i]); // + *ptr + fuga.uwaaaa + fugafuga->uwaaaa;
        array3[i] = array1[i] / array5[i];
        array4[i] = array3[i] - array2[i];
        i = array3[i] + !i - uhyooo + k;
        array3[i] += i + (float)N + 1.0f + teisuu + *ptrconst0 + *ptrconst1 + *ptrconst2 + *ptrconst3 + *ptrconst4;
        float* array6;
        float** pp;
        float arrayN[N];
        // float arrayKoko[2] = {1, 2};
        struct hoge fugaga;
        unsigned char ccc = 'a';
        unsigned char eee = ccc;
        i++;
        i+-1;
        if( uhyooo > 3 ) {
            ccc--;
        } 
        else if(1) {
            int x=1;
            x = 1;
        }        
        else {
            i--;
        }
        int k;
        double x1 = 2 * -x1;
    #pragma acc loop vector
        for( k=0; k<4; k++)
            for( l=0; l<4; l++)
                for(;;)
                    array3[k] += l;
                    // ppのようにオフロードブロックで動的確保するのは禁止にしたので、以下はそもそもコンパイルエラーになる
                        // ppを単なるポインタとして使うならコンパイルは通る
                    // array3[k] += pp[i][k];
        while(1){
            break;
        }
    }
// c++;
// ptr++;
// kansuu(&fuga);
// &sum;
}
// }

#pragma acc exit data copyout(array1[:N], array2[:N], array3[:N], array4[:N])

    for(int j=0; j<N; j++) {
        sum += array1[j];
    }

    // func_dummy(array1);

    printf("sum[0] = %lf\n", sum);

    return 0;
}