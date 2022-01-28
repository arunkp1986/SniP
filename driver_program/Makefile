CC=gcc
CFLAGS=-I.
DEPS = benchmark_run.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

benchmark_run: benchmark_run.o 
	$(CC) -o benchmark_run benchmark_run.o 

.PHONY: clean

clean:
	rm -f *.o benchmark_run 
