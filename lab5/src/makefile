CC=g++
CFLAGS=-I.
MUTEXGLAG=-DMAKE_WITH_MUTEX

mutex :
	$(CC) -o mut mutex.c -lpthread $(CFLAGS)

mutex_another :
	$(CC) -o mut2 mutex.c $(MUTEXGLAG) -lpthread $(CFLAGS)

parallel_factorial :
	$(CC) -o paral_fact parallel_factorial.cpp -lpthread $(CFLAGS)

clean :