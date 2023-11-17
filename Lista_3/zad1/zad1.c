#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int main() {
    int pid = getpid();
    printf("%d\n", getpid());
    int i = 0;
    struct timespec remaining, request;
    remaining.tv_nsec = 100;

    while (1){
        ++i;
        nanosleep(&request, &remaining); 
    }
    return 0;
}