#include "atomic.h"
#include "rwlock.h"

/* For various reader-writer lock algorithms, refer to
 * http://www.cs.rochester.edu/research/synchronization/pseudocode/rw.html
 * Current Linux rwlock is reader preference. */
 
/* The following implementation is based on the description of Intel TBB's
 * almost fair rwlock.
 * http://software.intel.com/en-us/blogs/2009/04/02/shared-access-with-tbb-readwrite-locks/ */

static const uint16_t TBB_WPENDING =  0x1;
static const uint16_t TBB_WRITER   =  0x2;
static const uint16_t TBB_CRITICAL = ~0x1;
static const uint16_t TBB_RINC     =  0x4; /* Reader increment */

void tbb_start_write(tbb_rwlock_t *l) {
    uint16_t tmp;
    while (1) {
        /* If there is someone in the critical section (CS) */
        if (l->counter & TBB_CRITICAL) {
            /* Set pending bit and then spin wait. Note we have to use locked
             * or, otherwise the reader finding the writer cut in and decrease
             * RINC may get lost. */
            atomic_or16((uint16_t *)l, TBB_WPENDING);
            while (l->counter & TBB_CRITICAL);
        }
        /* Try to acquire lock. If previous readers caused spin, pending bit is
         * set and no one will clear it. */
        tmp = l->counter & TBB_WPENDING;
        if (atomic_cmpxchg16((uint16_t *)l, tmp, TBB_WRITER) == tmp)
            return;
    }
}

void tbb_end_write(tbb_rwlock_t *l) {
    /* Why can't simply write a 0?
     * If both the reader and write try to enter CS, consider the following
     * interleaving:
     * 1. Writer's cmpxchg succeeds
     * 2. Reader executes xadd, finds that writer cuts in
     * 3. Writer done, simply write a 0
     * 4. Reader executes xadd with a negative value, reader counter becomes
     *    wrong.
     *
     * By clearing both the pending and writer bit, all the waiting reader and
     * writer have same chance to acquire the lock. */
    atomic_and16((uint16_t *)l, ~(TBB_WPENDING | TBB_WRITER));
}

void tbb_start_read(tbb_rwlock_t *l) {
    uint16_t lval;
    while (1) {
        /* If there's no writer or pending writer */
        if (! (l->counter & (TBB_WPENDING | TBB_WRITER))) {
            lval = atomic_fetch_and_add16((uint16_t *)l, TBB_RINC);
            /* If no writer cuts in before we xadd, we can enter CS since writers
             * will see a non zero reader count and wait */
            if (! (lval & (TBB_WPENDING | TBB_WRITER)))
                return;
            /* Otherwise, restore the read counter.
             * We can also spin wait the writer here, and it would perfer reader
             * a little more. */
            atomic_add16((uint16_t *)l, -TBB_RINC);
        }
    }
}

void tbb_end_read(tbb_rwlock_t *l) {
    atomic_add16((uint16_t *)l, -TBB_RINC);
}
