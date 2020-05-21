CC=g++
CFLAGS=-I.

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o paral_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

sumlibrary.o : sumlibrary.h
	$(CC) -o sumlibrary.o -c sumlibrary.c $(CFLAGS)

process_memory :
	$(CC) -o proc_mem process_memory.c $(CFLAGS)

zombie :
	$(CC) -o zomb zombie.c $(CFLAGS)

parallel_sum : utils.o sumlibrary.o
	$(CC) -o paral_sum utils.o sumlibrary.o parallel_sum.c $(CFLAGS) -lpthread

clean :
	rm utils.o find_min_max.o sumlibrary.o
