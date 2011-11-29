#define _GNU_SOURCE

#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>

unsigned long cfreq = 2400L * 1000L * 1000L; /* 2.4Ghz */

#define t2s(t) ((t)/cfreq)
#define t2ms(t) ((unsigned long)(((t) * 1000.0) / cfreq) % 1000)
static inline unsigned long
read_tsc(void)
{
    unsigned a, d;
    __asm __volatile("rdtsc":"=a"(a), "=d"(d));
    return ((unsigned long)a) | (((unsigned long) d) << 32);
}

int main(int argc, char *argv[])
{   
    
    unsigned long tsc;
    cpu_set_t mask;
    
    CPU_ZERO( &mask);
    CPU_SET(1, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask ) != 0)
        printf("WARNING: Could not set CPU Affinity, continuing...\n");
    
    tsc = read_tsc();

    if (argc > 1 && fork() == 0){
        if (execvp(argv[1], &argv[1]) == -1) {
            printf("executing command failed\n");
            return 0;
        }
    }
    wait();
    tsc = read_tsc() - tsc;
    printf("\nCYCLE: %lu\n", tsc);
    printf("TIME: %lu s %lu ms\n", t2s(tsc), t2ms(tsc));
    return 0;
}
