CFLAGS = -O2 -g

programs = test-rwlock test-atomic test-spinlock

all: $(programs)

test-rwlock: test-rwlock.o rwlock.o
	$(CC) $(CFLAGS) -lpthread $^ -o $@ 

test-atomic: test-atomic.o
	$(CC) $(CFLAGS) $^ -o $@ 

test-spinlock: test-spinlock.o
	$(CC) $(CFLAGS) -lpthread $^ -o $@

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(programs)
