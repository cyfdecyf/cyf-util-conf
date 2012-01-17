CFLAGS = -O2 -g -std=gnu99

programs = test-rwlock test-atomic test-spinlock test-spinlock-xchg \
		   test-spinlock-k42

all: $(programs)

test-rwlock: test-rwlock.o rwlock.o
	$(CC) $(CFLAGS) -lpthread $^ -o $@ 

test-atomic: test-atomic.o
	$(CC) $(CFLAGS) $^ -o $@ 

test-spinlock: test-spinlock.c
	$(CC) $(CFLAGS) -lpthread $^ -o $@

test-spinlock-xchg: test-spinlock.c
	$(CC) $(CFLAGS) -DXCHG -lpthread $^ -o $@

test-spinlock-k42: test-spinlock.c
	$(CC) $(CFLAGS) -DK42 -lpthread $^ -o $@

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(programs)
