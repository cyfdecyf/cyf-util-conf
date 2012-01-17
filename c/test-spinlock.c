#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "atomic.h"

#ifdef XCHG
#include "spinlock-xchg.h"
#elif defined(K42)
#include "spinlock-k42.h"
#else
#include "spinlock.h"
#endif

/* It's hard to say which spinlock implement performs best from the test I done
 * on Intel(R) Xeon(R) CPU E7- 4850  @ 2.00GHz, Dell r910 server. (4 CPUs, 10
 * cores each)
 * - For spinlock with cmpxchg, the performance degrades very fast with the
 * increase of threads. Seems that it does not have good scalability.
 * - For spinlock with xchg, it has much better scalability than cmpxchg, but it's
 * slower when there's 2 and 4 cores.
 * - For k42, The case for 2 threads is extremely bad, for other number of
 *   threads, the performance is stable and shows good scalability.
 */

/* Number of total lock/unlock pair.
 * Note we need to ensure the total pair of lock and unlock opeartion are the
 * same no matter how many threads are used. */
#define N_PAIR 16000000

static int nthr = 0;

static volatile int wflag;
/* Wait on a flag to make all threads start almost at the same time. */
void wait_flag(volatile uint32_t *flag, uint32_t expect) {
    atomic_inc32((uint32_t *)flag);
    while (*flag != expect) {
        cpu_relax();
    }
}

static struct timeval start_time;
static struct timeval end_time;

static void calc_time(struct timeval *start, struct timeval *end) {
    if (end->tv_usec < start->tv_usec) {
        end->tv_sec -= 1;
        end->tv_usec += 1000000;
    }

    assert(end->tv_sec >= start->tv_sec);
    assert(end->tv_usec >= start->tv_usec);
    struct timeval interval = {
        end->tv_sec - start->tv_sec,
        end->tv_usec - start->tv_usec
    };
    printf("%d.%06d\t", interval.tv_sec, interval.tv_usec);
}

volatile int counter = 0;
spinlock sl = SPINLOCK_INITIALIZER;

void *inc_thread(void *id) {
    int n = N_PAIR / nthr;
    assert(n * nthr == N_PAIR);
    wait_flag(&wflag, nthr);

    if (((long) id == 0)) {
        /*printf("get start time\n");*/
        gettimeofday(&start_time, NULL);
    }

    /* Start lock unlock test. */
    for (int i = 0; i < n; i++) {
        spin_lock(&sl);
        counter++;
        spin_unlock(&sl);
    }

    if (atomic_fetch_and_add32((uint32_t *)&wflag, -1) == 1) {
        /*printf("get end time\n");*/
        gettimeofday(&end_time, NULL);
    }
}

int main(int argc, const char *argv[])
{
    pthread_t *thr;
    int ret = 0;

    if (argc != 2) {
        printf("Usage: %s <num of threads>\n", argv[0]);
        exit(1);
    }

    nthr = atoi(argv[1]);
    /*printf("using %d threads\n", nthr);*/
    thr = calloc(sizeof(*thr), nthr);

    // Start thread
    for (long i = 0; i < nthr; i++) {
        if (pthread_create(&thr[i], NULL, inc_thread, (void *)i) != 0) {
            perror("thread creating failed");
        }
    }
    // join thread
    for (long i = 0; i < nthr; i++)
        pthread_join(thr[i], NULL);

    if (counter == N_PAIR) {
        /*printf("correct\n");*/
        calc_time(&start_time, &end_time);
        ret = 0;
    } else {
        printf("error\n");
        ret = 1;
    }

    return ret;
}
