#include "atomic.h"

int main(int argc, const char *argv[])
{
    volatile int cnt = 0;
    for (int i = 0; i < 100000000; i++) {
        atomic_add32((unsigned int *)&cnt, 1);
    }
    
    return 0;
}
