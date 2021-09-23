#include "csapp.h"

#define N 100   //#threads
#define M 50000 // #buckets

int ivals[M] = {-1};
int ihist[N] = {0}; // for each value of "i", how many threads have that value
int ireps[N] = {0};

// The cycle counter is a hardware register that gets incremented every
//    clock cycle.  We'll use it to generate index values, so that each thread
//    will (with high probability) get a different value.
// Assembly code to read cycle counter
void access_counter(unsigned *hi, unsigned *lo)
{
    /* Get cycle counter */
    asm("rdtsc; movl %%edx,%0; movl %%eax,%1"
        : "=r"(*hi), "=r"(*lo)
        : /* No input */
        : "%edx", "%eax");
}

int sample_index()
{
    unsigned hi, lo;
    access_counter(&hi, &lo);
    return lo % M;
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    int i = *((int *)vargp);
    int s = sample_index();
    ivals[s] = i;
    return NULL;
}

int main()
{
    int i, s, r, maxr = -1;
    int scount = 0;
    pthread_t tid;
    for (i = 0; i < N; ++i)
    {
        Pthread_create(&tid, NULL, thread, &i);
    }
    sleep(5);
    for (s = 0; s < M; s++)
    {
        if (ivals[s] > 0)
        {
            scount++;
            ihist[ivals[s]]++;
        }
    }
    maxr = -1;

    printf("Value\tCount\n");
    for (i = 0; i < N; i++)
    {
        r = ihist[i];
        ireps[r]++;
        if (r > maxr)
            maxr = r;
        printf("%d\t%d\n", i, ihist[i]);
    }

    printf("%d samples detected\n", scount);
    for (r = 0; r <= maxr; r++)
    {
        printf("%d\t%d\n", r, ireps[r]);
    }
    return 0;
}