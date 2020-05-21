#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    pid_t pid = 0;

    pid = fork();
    if (pid < 0)
        return -1;
    
    if (pid == 0) {
        sleep(10);
    }
    else {
        sleep(30);
    }

    return 0;
}