#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <getopt.h>
#include <cstdlib>

// Shared variables
int res = 1;
int mod = -1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

// Arguments struct
struct ThreadArgs {
    int k_begin;
    int k_end;
};

void* ThreadFunc(void* _args) {
    struct ThreadArgs args = *(struct ThreadArgs*)_args;
    int result = 1;

    for (int i = args.k_begin; i < args.k_end; i++) {
        result *= i;
        result %= mod;
    }
    
    pthread_mutex_lock(&mut);
    res *= result;
    res %= mod;
    pthread_mutex_unlock(&mut);

    return NULL;
}

int main(int argc, char** argv) {
    // Input variables
    int k = -1;
    int pnum = -1;

    // -----===== GETOPT ZONE =====-----
    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"factorial", required_argument, 0, 'k'},
                                         {"pnum", required_argument, 0, 0},
                                         {"mod", required_argument, 0, 0},
                                         {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "k:", options, &option_index);

        if (c == -1) break;

        switch (c) {
        case 0:
            switch (option_index) {
            case 0:
                k = atoi(optarg);
                // your code here
                // error handling
                break;
            case 1:
                pnum = atoi(optarg);
                // your code here
                // error handling
                break;
            case 2:
                mod = atoi(optarg);
                // your code here
                // error handling
                break;

            defalut:
                printf("Index %d is out of options\n", option_index);
            }
            break;
        
        case 'k':
            k = atoi(optarg);
            break;
        case '?':
            break;

        default:
            printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (k == -1 || pnum == -1 || mod == -1) {
        std::cout << k << '\t' << pnum << '\t' << mod << std::endl;
        printf("Usage: %s --factorial \"num\" --pnum \"num\" --mod \"num\"\n",
            argv[0]);
        return 1;
    }
    // -----===== GETOPT ZONE =====-----

    pthread_t* threads = new pthread_t[pnum];
    struct ThreadArgs* args = new ThreadArgs[pnum];

    for (int i = 0; i < pnum; i++) {
        args[i].k_begin = k * i / pnum + 1;
        args[i].k_end = k * (i + 1) / pnum + 1;
        if (pthread_create(&threads[i], NULL, ThreadFunc, (void*)(args + i)) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < pnum; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    std::cout << "Result: " << res << std::endl;

    delete[] threads;
    delete[] args;

    return 0;
}