#include <fcntl.h>
#define FIFO "/tmp/potok"
#include <stdio.h>

void main() {
    
    int potok_fd, licz;
    char bufor[BUFSIZ];
    potok_fd = open(FIFO, O_RDONLY);
    while ((licz=read(potok_fd, bufor,BUFSIZ)) > 0){
        write(1, bufor, licz);
    }

}
