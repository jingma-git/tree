#include <sys/time.h>
#include <cstring>
#include <iostream>
using namespace std;

const int N = 2048;

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void copyij(int src[N][N], int tgt[N][N])
{
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            tgt[i][j] = src[i][j];
}

void copyji(int src[N][N], int tgt[N][N])
{
    for (int j = 0; j < N; ++j)
        for (int i = 0; i < N; ++i)
            tgt[i][j] = src[i][j];
}

int src[N][N], tgt[N][N];

int main()
{

    memset(src, 1, sizeof(src));
    for (int K = 0; K < 32; ++K)
    {
        double t = get_time();
        copyij(src, tgt);
        // copyji(src, tgt);
        printf("%f\n", get_time() - t);
    }
    return 0;
}