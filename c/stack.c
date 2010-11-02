#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <spinlock.h>

/* Return 1 if swap happened. */
static inline unsigned int compare_and_swap(volatile void *address,
        void *old_value, void *new_value)
{
	void *prev = 0;

	asm volatile("lock; cmpxchgq %1,%2"
		: "=a"(prev)
		: "r"(new_value), "m"(*(long *)address), "0"(old_value)
		: "memory");

	return prev == old_value;
}

/*#define MUTEX*/
/*#define SPINLOCK*/

typedef struct Node {
    struct Node *next;
    int val;
} Node;

typedef struct {
    volatile Node *top;
#ifdef MUTEX
    pthread_mutex_t mutex;
#elif defined(SPINLOCK)
    Spinlock slock;
#endif
} Stack;

Stack gstack;

#if defined(MUTEX) || defined(SPINLOCK)

void push(Stack *stack, Node *n) {
#ifdef MUTEX
    pthread_mutex_lock(&stack->mutex);
#elif defined(SPINLOCK)
    spin_lock(&stack->slock);
#endif

    n->next = (Node *)stack->top;
    stack->top = n;

#ifdef MUTEX
    pthread_mutex_unlock(&stack->mutex);
#elif defined(SPINLOCK)
    spin_unlock(&stack->slock);
#endif
}

Node *pop(Stack *stack) {
    Node *oldtop;

    if (stack->top == NULL)
        return NULL;

#ifdef MUTEX
    pthread_mutex_lock(&stack->mutex);
#elif defined(SPINLOCK)
    spin_lock(&stack->slock);
#endif
    oldtop = (Node *)stack->top;
    stack->top = oldtop->next;

#ifdef MUTEX
    pthread_mutex_unlock(&stack->mutex);
#elif defined(SPINLOCK)
    spin_unlock(&stack->slock);
#endif

    return oldtop;
}

#else

/* Lock free version. */
void push(Stack *stack, Node *n) {
    Node *oldtop;
    while (1) {
        oldtop = (Node *)stack->top;
        n->next = oldtop;
        if (compare_and_swap(&stack->top, oldtop, n))
            return;
    }
}

Node *pop(Stack *stack) {
    Node *oldtop, *next;

    while (1) {
        oldtop = (Node *)stack->top;
        if (oldtop == NULL)
            return NULL;
        next = oldtop->next;
        if (compare_and_swap(&stack->top, oldtop, next))
            return oldtop;
    }
}

#endif

/* Testing code. */

#define NITERS 2000000
#define NTHR 3

void *producer(void *dummy) {
	long i, tid = (long) dummy;
	for (i = 0; i < NITERS; i++) {
		Node *n = malloc(sizeof(*n));
		n->val = NTHR * i + tid;
		push(&gstack, n);
	}
}

static inline void atomic_inc64(volatile unsigned long* address)
{
	asm volatile(
		"lock; incq %0\n\t"
		: "=m" (*address)
		: "m" (*address));
}

volatile unsigned long popcount = 0;

void *consumer(void *dummy) {
    Node *n;

    while (popcount < NTHR * NITERS) {
        n = pop(&gstack);
        if (n) {
            printf("%d\n", n->val);
            atomic_inc64(&popcount);
        }
    }
}

int main(int argc, const char *argv[]) {
#ifdef MUTEX
    pthread_mutex_init(&gstack.mutex, NULL);
#endif

#ifdef SPINLOCK
    SPIN_LOCK_INIT(&gstack.slock);
#endif

    /* TODO We need more threads to test the scalability of the stack
     * implementation.
     * 
     * With 3 producer calling push,
     * 1. spinlock give worst performance.
     * 2. mutex is faster than spinlock, but it's running time is not stable.
     * 3. lock free version is fast, and running time is stable.
     */

    pthread_t thr1, thr2, thr3, thr4, thr5;

    if (pthread_create(&thr1, NULL, producer, (void *)0) != 0) {
        perror("thread creating failed");
    }

    if (pthread_create(&thr2, NULL, producer, (void *)1) != 0) {
        perror("thread creating failed");
    }

    if (pthread_create(&thr3, NULL, producer, (void *)2) != 0) {
        perror("thread creating failed");
    }

/*
 *    if (pthread_create(&thr4, NULL, consumer, NULL) != 0) {
 *        perror("thread creating failed");
 *    }
 *
 *    if (pthread_create(&thr5, NULL, consumer, NULL) != 0) {
 *        perror("thread creating failed");
 *    }
 */

    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);
    pthread_join(thr3, NULL);
    /*
     *pthread_join(thr4, NULL);
     *pthread_join(thr5, NULL);
     */

    return 0;
}

