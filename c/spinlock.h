#ifndef _SPINLOCK_H
#define _SPINLOCK_H

typedef struct {
    volatile int lock;
} Spinlock;

#define SPINLOCK_ATTR static __inline __attribute__((always_inline, no_instrument_function))

SPINLOCK_ATTR int __testandset(Spinlock *p)
{
    int readval = 0;

    __asm__ __volatile__ (
			"lock; cmpxchgl %2, %0"
			: "+m" (p->lock), "+a" (readval)
			: "r" (1)
			: "cc");
    return readval;
}

SPINLOCK_ATTR void spin_lock(Spinlock *lock)
{
    while (__testandset(lock));
}

SPINLOCK_ATTR void spin_unlock(Spinlock *s)
{
    s->lock = 0;
}

#define SPIN_LOCK_INIT(l) spin_unlock(l)

#endif /* _SPINLOCK_H */
