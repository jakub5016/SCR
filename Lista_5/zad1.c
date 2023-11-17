#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    /**
     * Odczytuje plik tekstowy podany w argumencie wywołania (pełna nazwa wraz z formatem
     * plik np *.txt), następnie tworzy potok i wysyła do procesu potomnego zawartośc pliku
     * tekstowego. Należy podać nazwę pliku tekstowego oraz wielkość wysyłanego bufora w 
     * każdej iteracji.
    */
    if (argc != 3){
        printf("Zła ilosc argumentow\n");
        return 1;
    }

    int send_bufor = atoi(argv[2]);
    char * nazwa = argv[1];
    printf("Nazwa pliku: %s \nWielkosc bufora: %d\n\n", nazwa, send_bufor);

    int potok_fd[2], licz, status;
    char bufor[BUFSIZ];
    char c, result[send_bufor];
    pipe(potok_fd);
    
    if (fork() == 0) { // Dziecko

        close(potok_fd[1]); // Zamykamy wysyłanie z jednej strony 
    
        while ((licz=read(potok_fd[0], bufor, BUFSIZ)) > 0){
            write(1, bufor, licz);
        }
    }

    FILE * fp;
    fp = fopen(nazwa,"r");
    
    int counter=0; 
    while(1) {
        counter = (counter +1)%send_bufor;
        if( feof(fp) ) {break;}
        
        c = fgetc(fp);
        result[counter] = c;
        if (counter == (send_bufor-1)){
            write(potok_fd[1], "@@@@ ", 5);
            write(potok_fd[1], result, send_bufor);
            write(potok_fd[1], " ####\n", 6);  
        }

    }
    fclose(fp);
    exit(0);

    wait(&status);
    return(status);
}
