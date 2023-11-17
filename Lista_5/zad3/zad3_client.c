#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO "/tmp/potok"

int main() {
    /**
     * Skrypt odczytuje z potoku pod adresem /tmp/potok. 
     * Można używać zamiennie z komendą "tail -f /tmp/potok".
    */
    int potok_fd, licz;
    char bufor[100];
    potok_fd = open(FIFO, O_RDONLY);
    while ((licz=read(potok_fd, bufor,100)) > 0){
        write(1, bufor, licz);
    }
}