#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define FIFO "/tmp/potok"


int main(int argc, char **argv) {
    /**
     * Serwer odpowiadający za wysyłanie informacji do potoku.
     * Przed uruchomieniem programów serwera i klienta należy wywołać komendę: "mknod potok p",
     * tak aby został utworzony potok.
    */
    
    if (argc < 2){
        printf("Zła ilosc argumentow\n");
        return 1;
    }

    int potok_fd;
    potok_fd = open(FIFO, O_WRONLY);
    int counter = 1; // Zmienna do przechodenia po wywołaniach
    FILE * f;

    int send_bufor = 3;   
    char c;
    char result[send_bufor];
    int current_size = 0; // Jak dużo bufora już zapisano

    while (counter < argc)
    {
        f = fopen(argv[counter], "r");
        current_size = 0;

        while(1) { // Wysyłanie pliku w częsciach.
            if( feof(f)  || ((int)(c = fgetc(f)) == -49)) {write(potok_fd, result, current_size-1);break;}
            // -49 ponieważ znak pojawia sie znak � na koncu pliku tekstowego
            result[current_size] = c;
            
            if (current_size == (send_bufor-1)){
                write(potok_fd, result, send_bufor);
            }
            current_size = (current_size +1)%send_bufor;

        }
        fclose(f);
        counter++;
        sleep(1);

    }


    return 0;
}
