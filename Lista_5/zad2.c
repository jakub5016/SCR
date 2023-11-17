#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    /**
     * Należy podać w pierwszym wywołaniu nazwę pliku wraz z rozszerzeniem (*.png).
    */
    int potok_fd[2];
    if (argc < 2){
        printf("Niewystarczajaca ilosc argumentow\n");
        exit(-1);
    }

    int pipe_err = pipe(potok_fd); 
    if (pipe_err != 0){printf("Nie udalo sie utworzyc potoku\n");exit(-1);}


    int fork_err = fork();
    if (fork_err == -1){printf("Nie udalo sie sklonowac procesu\n");exit(-1);}
    else if (fork_err == 0) // Process dziecka
    {        
        close(potok_fd[1]);
        close(0);
        dup2(potok_fd[0], 1);
        execlp("feh", "feh","--reload","1", NULL);
        exit(0);
    }
    // Process rodzica

    FILE * img_file  = fopen(argv[1], "rb");
    int read_bytes; 
    char buffer[BUFSIZ];

    while ((read_bytes = fread(buffer, 1, sizeof(buffer), img_file))>0)
    {
        write(potok_fd[1], buffer, read_bytes);
    }
    

    // write(potok_fd[1], img_file, 1);
    printf("Plik został wyslany\n");

    wait(NULL);
    exit(0);
}
