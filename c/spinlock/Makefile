CFLAGS = -O2 -g -std=gnu99 -I..

programs = test-spinlock-cmpxchg test-spinlock-xchg test-spinlock-k42 \
		   test-spinlock-mcs test-spinlock-ticket test-spinlock-pthread \
		   test-spinlock-xchg-backoff

all: $(programs)

test-spinlock-cmpxchg: test-spinlock.c
	$(CC) $(CFLAGS) -DCMPXCHG -lpthread $^ -o $@

test-spinlock-xchg: test-spinlock.c
	$(CC) $(CFLAGS) -DXCHG -lpthread $^ -o $@

test-spinlock-k42: test-spinlock.c
	$(CC) $(CFLAGS) -DK42 -lpthread $^ -o $@

test-spinlock-mcs: test-spinlock.c
	$(CC) $(CFLAGS) -DMCS -lpthread $^ -o $@

test-spinlock-ticket: test-spinlock.c
	$(CC) $(CFLAGS) -DTICKET -lpthread $^ -o $@

test-spinlock-pthread: test-spinlock.c
	$(CC) $(CFLAGS) -DPTHREAD -lpthread $^ -o $@

test-spinlock-xchg-backoff: test-spinlock.c
	$(CC) $(CFLAGS) -DXCHGBACKOFF -lpthread $^ -o $@

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(programs)
