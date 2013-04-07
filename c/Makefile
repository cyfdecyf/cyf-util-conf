CFLAGS = -O2 -g -std=gnu99

programs = test-atomic

all: $(programs)

test-atomic: test-atomic.o
	$(CC) $(CFLAGS) $^ -o $@ 

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(programs)
