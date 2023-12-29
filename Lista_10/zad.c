#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t compute;

struct proscess_info
{
    int number;
    int exec_time;
};

void * SJF(){

}

void * FCFS(void * arg){ // First come first served
    struct proscess_info * data = (struct proscess_info *)arg;


    pthread_mutex_lock(&compute);
    int pocess_number_got = data->number;
    printf("Proces nr.%d zostal uruchomiony\n", pocess_number_got);
    pthread_mutex_unlock(&compute);
    return NULL;
}

int main(int argc, char *argv[]) {// Argumenty wywolania 
    if (argc < 3){
        printf("Bad arg val\n");
        return -1;
    }
    int ilosc_watkow = atoi(argv[2]);
    pthread_t watki[ilosc_watkow];
    int rodzaj_strategii = atoi(argv[1]);

    printf("ILOSC WATKOW: %d\nRODZAJ STRATEGII : %d\n", ilosc_watkow, rodzaj_strategii);

    pthread_mutex_init(&compute, NULL);

    struct proscess_info info;

    for(int i =0; i< ilosc_watkow; i++){
        info.number = i;

        // printf("rodzaj_strategii: %d\n", rodzaj_strategii);
        switch (rodzaj_strategii)
        {
        case 0:
            pthread_create(&watki[i], NULL, FCFS, (void *) & info);
            break;
        
        case 1:
            info.exec_time = i * i; //EXAMPLE
            pthread_create(&watki[i], NULL, SJF, (void *) & info );
            break;
        default:
            printf("OKRESLONA STRATEGIA NIE ISTNIEJE\n");
            return -1;
            break;
        }
    }

    for(int i =0; i< ilosc_watkow; i++){
        pthread_join(watki[i], NULL);
    }    
    return 0;
}