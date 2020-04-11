#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {

    pid_t pid = fork();

    if (pid == 0) {
        execl("seq_min_max", "seq_min_max", "123", "16000", NULL);
    }
    else {
        printf("Parent\n");
    }

    return 0;
}