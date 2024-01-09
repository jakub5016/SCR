#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMMOUT 10

struct taczka
{
    int id, weight, ammout;
    char rockType[100];
};


struct taczka tablicaTaczek[1000];
int pozycjaNaTablicy = 0;

int main(){
    char buffer[1000];
    FILE *fptr;
    fptr = fopen("dane.txt","read");

    struct taczka pomocnicza;

    int minute = 0;
    int type = 0;


    while (fgets(buffer,sizeof(buffer), fptr) != NULL)
    {
        minute = buffer[0] - '0';
        printf("MINUTA: %d\n", minute);
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

                    printf("    ID = %d uzyskany z: %c oraz %c\n", pomocnicza.id, buffer[i],  buffer[i-1]);
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
                    printf("    Typ kamienia:%s\n", pomocnicza.rockType);
                    i--;
                    break;

                case 2: // Szukamy wagi
                    pomocnicza.weight = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        pomocnicza.weight = (pomocnicza.weight*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    printf("    Waga = %d uzyskany z: %c oraz %c\n", pomocnicza.weight, buffer[i],  buffer[i-1]);

                    break;
                
                case 3: // Szukamy ilości i KOŃCZYMY PRZESZUKIWANIE TEGO KAMULCA
                    pomocnicza.ammout = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        pomocnicza.ammout = (pomocnicza.ammout*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    printf("    Ilosc kamieni = %d uzyskany z: %c oraz %c\n", pomocnicza.ammout, buffer[i],  buffer[i-1]);
                    /// WYWAL TĄ TACZKEEEEEEEEEEE

                    tablicaTaczek[pozycjaNaTablicy] = pomocnicza;
                    pozycjaNaTablicy++;
                    printf("\n");

                    break;
                default:
                    break;
                }
            }
            else{
                type = (type +1)%4;
            }
        }

        printf("\n\n");

    }

    for (int i=0; i< pozycjaNaTablicy; i++){
        printf("Taczka o ID = %d zawierająca %s jest w tablicy pod numerem: %d\n", tablicaTaczek[i].id, tablicaTaczek[i].rockType, i);
    }

    return 0;
}