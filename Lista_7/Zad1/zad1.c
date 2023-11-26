#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <math.h>
#include <stdlib.h>



pthread_barrier_t barrier;


void * hello(void* ID){
    int wynik = 0;
    pthread_barrier_wait(&barrier);

    for (int i=0; i<5; ++i) wynik += log(i);
    printf("Hello OpSys. Written by thread %ld\n", ID);

    pthread_exit(0);
}

int main(int argc, char *argv[] ){
    if (argc < 2){
        printf("Brak arguemntu wywolania, podaj ilosc watkow \n");
        return -1;
    }

    int thread_ammount = atoi(argv[1]);
    pthread_t newThread[thread_ammount];
    pthread_barrier_init(&barrier, NULL, thread_ammount);

    for (int j = 0; j < thread_ammount; j++){
    pthread_create(&newThread[j], NULL, hello, (void*) j);
    }
    for (int j = 0; j < thread_ammount; j++){
        pthread_join(newThread[j], NULL);
    }

    pthread_exit(0);

    return 0;
}