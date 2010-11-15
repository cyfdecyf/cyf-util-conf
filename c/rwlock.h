#ifndef _RWLOCK_H
#define _RWLOCK_H

#include <stdint.h>

/* The counter should be initialized to be 0. */
typedef struct tbb_rwlock_t {
    volatile uint16_t counter;
} tbb_rwlock_t;

void tbb_start_write(tbb_rwlock_t *l);
void tbb_end_write(tbb_rwlock_t *l);

void tbb_start_read(tbb_rwlock_t *l);
void tbb_end_read(tbb_rwlock_t *l);

#endif /* _RWLOCK_H */
