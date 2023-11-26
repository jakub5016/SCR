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
pthread_mutex_t synowie_mutex;
pthread_mutex_t wyslanie_sygnalu;
pthread_cond_t synowie_cond; // Sygnalizator dla watkow
bool rejent_czeka = false;
char trafione_pozycje_syna[S][2]; // 2 poniewaz jest to X i Y

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
        printf("Syn %d, miejsce_X: %d, miejsce_Y: %d\n", numer_syna, miejsce_X, miejsce_Y);
        
        if (wioski[miejsce_X][miejsce_Y] == false){
            trafione_pozycje[i][0] = miejsce_X;
            trafione_pozycje[i][1] = miejsce_Y;
        }
    }
    pthread_mutex_lock(&synowie_mutex);
    
    while (!rejent_czeka)
    {
        sleep(1);
    }
    
    pthread_cond_signal(&synowie_cond); //Zasygnalizuj dodanie nowego komunikatu 
    
    printf("Syn wyslal sygnal\n");
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
    pthread_mutex_lock(&wyslanie_sygnalu);

    while (ilosc_synow <3)
    {
        printf("Rejent czeka na kolejnego syna\n");
        rejent_czeka = true;
        pthread_cond_wait(&synowie_cond, &synowie_mutex);
        rejent_czeka = false;
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
    pthread_mutex_init(&wyslanie_sygnalu, NULL);
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
        printf("Syn wrocil do domu\n");
    }

    pthread_mutex_unlock(&wyslanie_sygnalu);
    pthread_join(rejent_thread, NULL);

    return 0;
}