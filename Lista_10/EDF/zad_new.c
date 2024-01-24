#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMMOUT 10
#define MAXROBOTS 10

int iloscRobotow = 0;
int kwantCzasu = 0;
int typAlgorytmu = 2; //FCFS

// FCFS Var
int lastTaczka = 0;
// RR Var
int kwantCzasuCounters[MAXROBOTS];
// EDF Var
int zajetaTaczka[1000];
struct taczka
{
    int id, weight, ammout, timeToUnload;
    char rockType[100];
};

void printfTaczka(struct taczka pomocnicza){
    printf("    wheelbarrow arrived <%d %s %d %d [%d]>\n", pomocnicza.id, pomocnicza.rockType, pomocnicza.weight, pomocnicza.ammout, pomocnicza.weight * pomocnicza.ammout);
}

struct taczka tablicaTaczek[1000];
int pozycjaNaTablicy = 0;

void pop(int number){
    pozycjaNaTablicy = pozycjaNaTablicy -1;
    // printf("%d", pozycjaNaTablicy);
    for (int i = number; i <= pozycjaNaTablicy; i++)
    {
        tablicaTaczek[i] = tablicaTaczek[i+1];
    }
    
}

void switch_table(int index_i, int intex_j){
    struct taczka pomocnicza;
    pomocnicza = tablicaTaczek[intex_j];
    tablicaTaczek[intex_j] = tablicaTaczek[index_i];
    tablicaTaczek[index_i] = pomocnicza;
}

void EDF(){
    printf("\n");
    printf("------------------------MOMENT DZIAŁANIA SCHEDULERA------------------------\n");

    for (int l =iloscRobotow; l < pozycjaNaTablicy; l++){
        for (int m =l; m < pozycjaNaTablicy; m++){
            if(tablicaTaczek[l].id < tablicaTaczek[m].id){
                switch_table(l, m);
            }
        }
    }


    for (int h =0; h < pozycjaNaTablicy; h++){
        if (tablicaTaczek[h].timeToUnload == 0){
            tablicaTaczek[h].id = -1;
            switch_table(h, iloscRobotow);

        }
    }


    for (int j = 0; j< iloscRobotow; j++){
        for (int k = iloscRobotow; k <= pozycjaNaTablicy; k++){
            if(tablicaTaczek[j].id < tablicaTaczek[k].id){
                switch_table(j, k);
            }
        }
    }

    printf("        ");
    for (int i = 0; i < iloscRobotow; i++){
        if (tablicaTaczek[i].timeToUnload != 0){
            printf("[%s        %d]",tablicaTaczek[i].rockType, tablicaTaczek[i].timeToUnload);
            tablicaTaczek[i].timeToUnload = tablicaTaczek[i].timeToUnload -1;

        }
        else{
            printf("[        ]");
        }
        
    }
    printf("\n\n");
}

void RR(){
    struct taczka pomocnicza;
    printf("------------------------MOMENT DZIAŁANIA SCHEDULERA------------------------\n");

    for (int j =0; j< iloscRobotow; j++){
        if ((kwantCzasu-1) == kwantCzasuCounters[j]){
            for (int i = 0; i < iloscRobotow; i++)
            {
                if (i < pozycjaNaTablicy){
                    if (tablicaTaczek[0].timeToUnload != 0){
                        pomocnicza = tablicaTaczek[0];
                        pop(0);
                        tablicaTaczek[pozycjaNaTablicy] = pomocnicza;
                        pozycjaNaTablicy++; 
                    }
                    else{
                            pop(0); 
                    }
                }
            }

        }
    }

    for (int k = 0; k < iloscRobotow; k++)
    {
        kwantCzasuCounters[k] = (kwantCzasuCounters[k] +1)%kwantCzasu;
    }

    printf("        ");
    for (int i = 0; i < iloscRobotow; i++){
        if ((i < pozycjaNaTablicy) && (tablicaTaczek[i].timeToUnload != 0)){
            printf("[%s        %d]",tablicaTaczek[i].rockType, tablicaTaczek[i].timeToUnload);
            tablicaTaczek[i].timeToUnload = tablicaTaczek[i].timeToUnload -1;

            if (tablicaTaczek[i].timeToUnload == 0){
                kwantCzasuCounters[i] = (kwantCzasu -1);                   
            }
        }        
        else{
            printf("[        ]");
        }
        
    }
          
    printf("\n\n");
}

void FCFS(){
    printf("        ");
    for (int i = 0; i < iloscRobotow; i++){
        if ((lastTaczka + i) < pozycjaNaTablicy){
            while ((tablicaTaczek[lastTaczka + i].timeToUnload == 0) && ((lastTaczka + i) < pozycjaNaTablicy)) {
                lastTaczka++;
            }


            printf("[%s        %d]",tablicaTaczek[lastTaczka + i].rockType, tablicaTaczek[lastTaczka + i].timeToUnload);
            tablicaTaczek[lastTaczka + i].timeToUnload = tablicaTaczek[lastTaczka + i].timeToUnload -1;

        }
        else{
            printf("[        ]");
        }
        
    }
    printf("\n\n");
}

int main(int argc, char *argv[]){
    if (argc < 4){
        printf("Niewystarczajaca ilosc argumentow wywoalania\n");
        return -1;
    }

    // Przygotowanie tablicy
    for (int i = 0; i < 1000; i++){
        tablicaTaczek[i].timeToUnload = 0;
        tablicaTaczek[i].id = -1; // Ustawienie ID specjalnego
        zajetaTaczka[i] = 0;
    }

    iloscRobotow = atoi(argv[1]);
    printf("%d robots in the mine\n", iloscRobotow);
    kwantCzasu = atoi(argv[2]);
    FILE *fptr;
    if (strcmp(argv[3], "-")){
        fptr = fopen(argv[3],"read");
        if (fptr == NULL){
            printf("Nie istnieje plik o takiej nazwie\n");
            return -1;
        }
    }
    else{
        fptr = stdin;
    }
    typAlgorytmu = atoi(argv[4]);
    if (typAlgorytmu == 1){
        printf("RR with quantum %d\n", kwantCzasu);        
        for (int i = 0; i < MAXROBOTS; i++)
        {
            if (i >iloscRobotow){
                kwantCzasuCounters[i] = -1;
            }
            else{
                kwantCzasuCounters[i] = 0;
            }
        }
        

    }

    char buffer[1000];
    struct taczka pomocnicza;

    int minute = 0;
    int type = 0;

    while (fgets(buffer,sizeof(buffer), fptr) != NULL)
    {
        int i = 2;
        minute = buffer[0] - '0';
        if ((((buffer[1]) != ' ') && (buffer[1] != '\n')) && (buffer[1] != '\0')){
            minute = (minute*10) + (buffer[1] - '0');
            i=3;
        }
        printf("Moment %d\n", minute);
        type = 0;
        for (; ; i++){
            if ((buffer[i] == '\0') || (buffer[i] == '\n')){
                break;
            }


            if (buffer[i] != ' '){
                switch (type)
                {
                case 0: // Szukamy ID
                    pomocnicza.id = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        pomocnicza.id = (pomocnicza.id*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    break;

                case 1: // Szukamy slowa
                    int j = 0;
                    while (buffer[i] != ' ')
                    {
                        pomocnicza.rockType[j] = buffer[i];
                        i++;
                        j++;
                    }
                    pomocnicza.rockType[j] = '\0';
                    i--;
                    break;

                case 2: // Szukamy wagi
                    pomocnicza.weight = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        pomocnicza.weight = (pomocnicza.weight*10) + (buffer[i+1] - '0');
                        i++;
                    }


                    break;
                
                case 3: // Szukamy ilości i KOŃCZYMY PRZESZUKIWANIE TEGO KAMULCA
                    pomocnicza.ammout = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        pomocnicza.ammout = (pomocnicza.ammout*10) + (buffer[i+1] - '0');
                        i++;
                    }
                    /// WYWAL TĄ TACZKEEEEEEEEEEE

                    pomocnicza.timeToUnload = pomocnicza.ammout * pomocnicza.weight;
                    tablicaTaczek[pozycjaNaTablicy] = pomocnicza;
                    pozycjaNaTablicy++;
                    printfTaczka(pomocnicza);
                    break;
                default:
                    break;
                }
            }
            else{
                type = (type +1)%4;
            }
        }
        switch (typAlgorytmu)
        {
        case 2:
            FCFS();
            break;
        case 1:
            RR();
            break;
        case 3:
            EDF();
        default:
            break;
        }
    }

    int isEmpty = 0;
    if (typAlgorytmu < 1 || typAlgorytmu > 3){
        return -1;
    }
    while ((isEmpty == 0) && (minute < 100)) // Drugi argument to ułatwienie dla debugowania.
    {
        minute++;
        isEmpty = 1;

        if ((typAlgorytmu == 1) || (typAlgorytmu == 3)){
            for (int i = 0; i < pozycjaNaTablicy; i++)
            {
                if (tablicaTaczek[i].timeToUnload != 0){
                    isEmpty = 0;
                }
            }
        }
        else if (typAlgorytmu==2){
            if (tablicaTaczek[pozycjaNaTablicy-1].timeToUnload != 0){
                isEmpty = 0;
            }
        }

        if (isEmpty == 0){
            printf("Moment %d\n", minute);
            switch (typAlgorytmu)
            {
            case 2:
                FCFS();
                break;
            case 1:
                RR();
                break;
            case 3:
                EDF();
            default:
                break;
            }
        }
        
    }
    
    return 0;
}