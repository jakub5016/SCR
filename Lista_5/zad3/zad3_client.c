#include <fcntl.h>
#define FIFO "/tmp/potok"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    
    int potok_fd, licz;
    char bufor[BUFSIZ];
    potok_fd = open(FIFO, O_RDONLY);
    while ((licz=read(potok_fd, bufor,BUFSIZ)) > 0){
        write(1, bufor, licz);
    }

}
