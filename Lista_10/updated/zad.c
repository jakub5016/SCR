#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AMMOUT 10


int main(){
    char buffer[1000];
    FILE *fptr;
    fptr = fopen("dane.txt","read");

    int minute = 0;
    int type = 0;
    int id, weight, ammout;
    char rockType[100];

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
                    id = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        id = (id*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    printf("    ID = %d uzyskany z: %c oraz %c\n", id, buffer[i],  buffer[i-1]);
                    break;

                case 1: // Szukamy slowa
                    int j = 0;
                    while (buffer[i] != ' ')
                    {
                        rockType[j] = buffer[i];
                        i++;
                        j++;
                    }
                    rockType[j] = '\0';
                    printf("    Typ kamienia:%s\n", rockType);
                    i--;
                    break;

                case 2: // Szukamy wagi
                    weight = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        weight = (id*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    printf("    Waga = %d uzyskany z: %c oraz %c\n", weight, buffer[i],  buffer[i-1]);

                    break;
                
                case 3: // Szukamy ilości i KOŃCZYMY PRZESZUKIWANIE TEGO KAMULCA
                    ammout = buffer[i] - '0';
                    if ((((buffer[i + 1]) != ' ') && (buffer[i+1] != '\n')) && (buffer[i+1] != '\0')){
                        ammout = (id*10) + (buffer[i+1] - '0');
                        i++;
                    }

                    printf("    Ilosc kamieni = %d uzyskany z: %c oraz %c\n", ammout, buffer[i],  buffer[i-1]);


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
    
    return 0;
}