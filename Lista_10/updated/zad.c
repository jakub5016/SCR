#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMMOUT 10

int iloscRobotow = 0;
int kwantCzasu = 0;
int typAlgorytmu = 2; //FCFS

// FCFS Var
int lastTaczka = 0;
// RR Var


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

    char buffer[1000];
    struct taczka pomocnicza;

    int minute = 0;
    int type = 0;


    while (fgets(buffer,sizeof(buffer), fptr) != NULL)
    {
        minute = buffer[0] - '0';
        printf("Moment %d\n", minute);
        type = 0;
        for (int i = 2; ; i++){
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
        
        default:
            break;
        }
    }

    int isEmpty = 0;
    if (typAlgorytmu < 1 || typAlgorytmu > 2){
        return -1;
    }
    while (isEmpty == 0)
    {
        minute++;
        isEmpty = 1;
        for (int i = 0; i < pozycjaNaTablicy; i++)
        {
            if (tablicaTaczek[i].timeToUnload != 0){
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
            
            default:
                break;
            }
        }
        
    }
    
    return 0;
}