#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

bool blocking = false;


void handler(int signal){
    switch (signal)
    {
    case SIGALRM:

        break;
    
    case SIGTERM:
        printf("Dziekuję za wysłanie sygnału SIGTERM, popełniam samobójstwo...\n");
        exit(0);
        break;

    case SIGUSR1:
        printf("Dziekuję za wysłanie sygnału SIGUSR1, wracam do pracy...\n");
        break;

    case SIGUSR2: // Blokuje użycie SIGUSR1
        blocking = !blocking;
        break;
    
    default:
        break;
    }
}

int main() {
    int pid = getpid();

    printf("Moj PID:%d \n", pid);

    signal(SIGALRM, handler);
    signal(SIGTERM, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    sigset_t    signal_set;
    int full_set = sigemptyset(&signal_set);
    int allow_set = sigaddset(&full_set, SIGUSR1);


    int counter = 0;
    
    while (1){
        if (blocking){
            sigprocmask(SIG_SETMASK, &allow_set, NULL);
            printf("Odblokowuje sygnał\n");
            sigprocmask(SIG_SETMASK, &full_set, NULL);
            printf("Blokuje sygnał\n");
        }
        sleep(3);

    }
    

}   