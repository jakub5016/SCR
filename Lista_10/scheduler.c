#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct proscess_info
{
    int number;
    int exec_time;
    int priority;
};


int main(int argc, char* argv[]){
    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("%s\n", buffer);
        printf("\n");
    }

    return 0;   

}