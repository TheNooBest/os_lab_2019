#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void* Print1(void* args) {
    cout << "Enter func \"Print1\"" << endl;

    pthread_mutex_lock(&mut1);
    cout << "\"Print1\" lock 1st mutex" << endl;

    sleep(1);

    pthread_mutex_lock(&mut2);
    cout << "\"Print1\" lock 2st mutex" << endl;

    pthread_mutex_unlock(&mut2);
    pthread_mutex_unlock(&mut1);

    return NULL;
}
void* Print2(void* args) {
    cout << "Enter func \"Print2\"" << endl;

    pthread_mutex_lock(&mut2);
    cout << "\"Print2\" lock 2st mutex" << endl;

    sleep(1);

    pthread_mutex_lock(&mut1);
    cout << "\"Print2\" lock 1st mutex" << endl;

    pthread_mutex_unlock(&mut1);
    pthread_mutex_unlock(&mut2);

    return NULL;
}

int main(int argc, char** argv) {
    pthread_t t1;
    pthread_t t2;

    
    if (pthread_create(&t1, NULL, Print1, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&t2, NULL, Print2, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(t1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    
    return 0;
}