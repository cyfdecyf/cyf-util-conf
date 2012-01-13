#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include "atomic.h"
#include "spinlock.h"

// Number of iteration in each thread
#define NITER 5000000
// Number of threads
#define NTHR 4

static volatile int wflag;
// Wait on a flag.
void wait_flag(volatile uint32_t *flag, uint32_t expect) {
    atomic_inc32((uint32_t *)flag);
    while (*flag != expect);
}

volatile int counter = 0;
Spinlock sl = SPINLOCK_INITIALIZER;

void *inc(void *dummy) {
    int i;
    wait_flag(&wflag, NTHR);

    for (i = 0; i < NITER; i++) {
        spin_lock(&sl);
        counter++;
        spin_unlock(&sl);
    }
}

int main(int argc, const char *argv[])
{
    pthread_t thr[NTHR];
    int i;

    // Start thread
    for (i = 0; i < NTHR; i++) {
        if (pthread_create(&thr[i], NULL, inc, NULL) != 0) {
            perror("thread creating failed");
        }
    }
    // join thread
    for (i = 0; i < NTHR; i++)
        pthread_join(thr[i], NULL);

    if (counter == NITER * NTHR) {
        printf("correct\n");
    } else {
        printf("error\n");
    }

    return 0;
}
