CFLAGS = -O2 -g -std=gnu99 -I..

programs = test-spinlock test-spinlock-xchg test-spinlock-k42

all: $(programs)

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
