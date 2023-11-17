#include <stdio.h>
#include <stdlib.h>

int main(){
    char * logname =  getenv("LOGNAME");
    printf("Hello %s\n", logname);
    return 0;

}