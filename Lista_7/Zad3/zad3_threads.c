#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>


int N = 0;
int sprawdz = 1;
int znalezione = 0;
int suma = 0;

pthread_mutex_t sprawdzenie; 
pthread_mutex_t wpisanie_sumy; 
pthread_mutex_t odczyt_znalezionej; 

void * szukanie_liczb_pierwszych(){
    int znaleziona_lokalnie = 0;

    while (znaleziona_lokalnie < N){
        pthread_mutex_lock(&sprawdzenie); 
        sprawdz++;
        int sprawdzana_liczba = sprawdz;
        pthread_mutex_unlock(&sprawdzenie); 

        bool czy_pierwsza = true;
        // Sprawdzanie czy liczba jest pierwsza
        for (int i = 2; i < sprawdzana_liczba; i++) {
            if (sprawdzana_liczba % i ==0){
                // printf("%d to nie jest liczba pierwsza, jest podzielne dla i = %d\n", sprawdz, i);
                czy_pierwsza = false;
                break;
            }
        }

        // Jeżeli tak sprawdź warunki
        if (czy_pierwsza){
            // printf("%d jest liczbą pierwsza.\n", sprawdzana_liczba);
            
            pthread_mutex_lock(&wpisanie_sumy); 
            
            if (znalezione < N){
                suma += sprawdzana_liczba;
                znalezione ++;
            }
            znaleziona_lokalnie = znalezione;
            pthread_mutex_unlock(&wpisanie_sumy); 
            
        }
    }
    return NULL;
}

int main(int argc, char *argv[] ){
    if (argc < 3){
        printf("Brak arguemntu wywolania, podaj ile liczb pierwszych ma znaleźć oraz ile wątków\n");
        return -1;
    }

    N = atoi(argv[1]);
    int threds_ammount = atoi(argv[2]);

    pthread_t Threads[threds_ammount];
    pthread_mutex_init(&sprawdzenie, NULL);
    pthread_mutex_init(&wpisanie_sumy, NULL);
    pthread_mutex_init(&wpisanie_sumy, NULL);

    int * N_pointer = &N;

    for (int j =0; j<threds_ammount; j++){
        pthread_create(&Threads[j], NULL, szukanie_liczb_pierwszych, NULL);
    }

    for (int j =0; j<threds_ammount; j++){
        pthread_join(Threads[j], NULL);
    }

    printf("Suma dla %d liczb pierwszych jest równa: %d\n", N, suma);

    pthread_mutex_destroy(&sprawdzenie);
    pthread_mutex_destroy(&wpisanie_sumy);

    return 0;
}