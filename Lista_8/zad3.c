#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#define X 10
#define Y 10
#define N 3
#define S 5

bool wioski[X][Y];
int licznik_sygnalow =1;
char trafione_pozycje_syna[S][2]; // 2 poniewaz jest to X i Y
bool rejent_zajal_mutex = false;

pthread_mutex_t synowie_mutex;
pthread_cond_t synowie_cond; // Sygnalizator dla watkow

void * syn(void * numer_syna){
    char trafione_pozycje[S][2];

    double numer_X = drand48();
    char miejsce_X = numer_X * X;

    double numer_Y = drand48();
    char miejsce_Y = numer_Y * Y;

    for (int i=0; i < S; i++){
        numer_X = drand48();
        miejsce_X = numer_X * X;

        numer_Y = drand48();
        miejsce_Y = numer_Y * Y;        
        if (wioski[miejsce_X][miejsce_Y] == false){
            trafione_pozycje[i][0] = miejsce_X;
            trafione_pozycje[i][1] = miejsce_Y;
        }
    }

    while (!rejent_zajal_mutex)
    {
        sleep(1);
    }
    

    pthread_mutex_lock(&synowie_mutex);
    printf("Licznik sygnalow teraz jest rowny: %d \n", licznik_sygnalow);

    while (licznik_sygnalow !=0)
    {
        sleep(1);
    }
    licznik_sygnalow++;

    pthread_cond_signal(&synowie_cond); //Zasygnalizuj dodanie nowego komunikatu 
    
    for (int i=0; i< S; i++){
        for (int j=0; j< 2; j++){
            trafione_pozycje_syna[i][j] = trafione_pozycje[i][j]; // Ustawiamy wszystkie miejsca na wolne
        }
    }

    pthread_mutex_unlock(&synowie_mutex);

    return NULL;
}
void * rejent(){
    int ilosc_synow = 0;
    pthread_mutex_lock(&synowie_mutex);
    rejent_zajal_mutex = true;
    while (ilosc_synow <3)
    {   
        printf("Rejent czeka na kolejnego syna\n");
        licznik_sygnalow = 0;
        
        pthread_cond_wait(&synowie_cond, &synowie_mutex);
        printf("Trafione przez tego syna pozycje to:\n");
        for (int i=0; i< S; i++){
            for (int j=0; j< 2; j++){
                if (j == 0){
                    printf("X: %d  ", trafione_pozycje_syna[i][j]);
                }
                else{
                    printf("Y: %d\n", trafione_pozycje_syna[i][j]);
                }
            }
        }    
        ilosc_synow ++;
    }
    printf("Koniec rejetna\n");

    pthread_mutex_unlock(&synowie_mutex);

    return NULL;
}


int main(int argc, char *argv[] ){
    pthread_mutex_init(&synowie_mutex, NULL);
    pthread_cond_init(&synowie_cond, NULL);


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