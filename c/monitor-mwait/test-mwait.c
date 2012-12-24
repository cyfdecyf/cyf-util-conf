#include "mwait.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int var;

void * write_thr(void *dummy) {
    // sleep 2 seconds to avoid mmwait miss the write.
    sleep(2);
    var = 1;
}

int main(int argc, const char *argv[])
{
    unsigned int eax, ebx, ecx, edx;
    eax = 0x1;
    cpuid(&eax, &ebx, &ecx, &edx);
    printf("after cpuid eax=0x1, eax=%#x, ecx=%#x\n", eax, ecx);
    if (ecx & (1 << 3) == 0) { // test bit 3 in ecx
        printf("monitor/mwait not supported or disabled\n");
        exit(1);
    }

    printf("setting up monitor address\n");
    monitor(&var, 0, 0);
    printf("monitor address setup done\n");

    pthread_t thr;
    if (pthread_create(&thr, NULL, write_thr, NULL) != 0) {
        perror("write thread creation failed");
        exit(1);
    }
    
    mwait(0, 0);

    return 0;
}

