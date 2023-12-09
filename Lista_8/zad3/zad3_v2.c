#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#define X 8
#define Y 7
#define N 5
#define S 5

bool wioski[X][Y];
int punkty_synow[N];
int licznik_sygnalow =1;
pthread_mutex_t synowie_mutex;
pthread_mutex_t zajmowanie_wioski;
pthread_cond_t synowie_cond; // Sygnalizator dla watkow
bool rejent_zajal_mutex = false;
int ilosc_synow = 0;


void * syn(void * numer_syna){
    double numer_X = drand48();
    char miejsce_X = numer_X * X;

    double numer_Y = drand48();
    char miejsce_Y = numer_Y * Y;

    int ile_trafionych = 0;
    for (int i=0; i < S; i++){
        numer_X = drand48();
        miejsce_X = numer_X * X;

        numer_Y = drand48();
        miejsce_Y = numer_Y * Y;    
        pthread_mutex_lock(&zajmowanie_wioski);
        if (wioski[miejsce_X][miejsce_Y] == false){
            wioski[i][0] = miejsce_X;
            wioski[i][1] = miejsce_Y;
            ile_trafionych++;
        }
        pthread_mutex_unlock(&zajmowanie_wioski);

    }
    int nr = numer_syna;
    punkty_synow[nr] = ile_trafionych;
    
    while (!rejent_zajal_mutex)
    {
        sleep(1);
    }

    pthread_mutex_lock(&synowie_mutex);
    pthread_cond_signal(&synowie_cond); //Zasygnalizuj dodanie nowego komunikatu 
    ilosc_synow ++;

    printf("Syn wyslal sygnal\n");
    pthread_mutex_unlock(&synowie_mutex);

    return NULL;
}
void * rejent(){
    pthread_mutex_lock(&synowie_mutex);
    while (ilosc_synow <N)
    {   
        printf("Rejent czeka na kolejnego syna\n");    
        rejent_zajal_mutex = true;
    
        pthread_cond_wait(&synowie_cond, &synowie_mutex);
        rejent_zajal_mutex = false;

        printf("Synowi udało się zając\n");
    }
    pthread_mutex_unlock(&synowie_mutex);
    system("clear");

    for (int i=0; i< X; i++){
        for (int j=0; j< Y; j++){
            if (wioski[i][j] == false){
                printf("[1] ");
            }
            else{
                printf("[0] ");
            }
        }
        printf("\n");
    }

    int suma_niezajetych =X*Y;
    for (int i=0; i< N; i++){
        printf("Syn %d zajął %d miejsc\n", i, punkty_synow[i]);
        suma_niezajetych = suma_niezajetych - punkty_synow[i];
    }
    printf("Suma niezajetych terytoriów: %d\n", suma_niezajetych);
    return NULL;
}


int main(int argc, char *argv[] ){
    pthread_mutex_init(&synowie_mutex, NULL);
    pthread_cond_init(&synowie_cond, NULL);
    pthread_cond_init(&zajmowanie_wioski, NULL);


    int PID = getpid();
    seed48(&PID);
    for (int i=0; i< X; i++){
        for (int j=0; j< Y; j++){
            wioski[i][j] = false; // Ustawiamy wszystkie miejsca na wolne
        }
    }
    pthread_t synowie_thread[N];
    pthread_t rejent_thread;
    
    pthread_create(&rejent_thread, NULL, rejent, NULL);

    for (int j =0; j<N; j++){
        pthread_create(&synowie_thread[j], NULL, syn, (void *) j);
    }

    for (int j =0; j<N; j++){
        pthread_join(synowie_thread[j], NULL);
    }

    pthread_join(rejent_thread, NULL);
    return 0;
}