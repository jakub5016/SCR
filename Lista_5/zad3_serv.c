#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define FIFO "/tmp/potok"
#define MESS \
"To jest komunikat serwera\n"

int main(int argc, char **argv) {
    /**
     * Serwer odpowiadający za wysyłanie informacji do potoku.
     * Przed uruchomieniem programów serwera i klienta należy wywołać komendę: "mknod potok p",
     * tak aby został utworzony potok.
    */
    int potok_fd;
    potok_fd = open(FIFO, O_WRONLY);
    int counter = 1; // Zmienna do przechodenia po wywołaniach
    FILE * f;
    
    if (argc < 2){
        printf("Zła ilosc argumentow\n");
        return 1;
    }

    while (counter < argc)
    {
        int send_bufor = 3;   
        char c;
        char result[send_bufor];
        int current_size = 0; // Jak dużo bufora już zapisano

        f = fopen(argv[counter], "r");
        while(1) {
            current_size = (current_size +1)%send_bufor;
            if( feof(f) ) {break;}
            
            c = fgetc(f);
            result[current_size] = c;
            if (current_size == (send_bufor-1)){
                write(potok_fd, result, send_bufor);
        }

    }
    }

    return 0;
}
