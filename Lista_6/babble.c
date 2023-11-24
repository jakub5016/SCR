#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

#define BABBLE_NAME "/scr"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

#define USER_NAME "Jakub Gil: "

struct babblespace {
    pthread_mutex_t babble_mutex;
    pthread_cond_t babble_cond;
    int babble_first, babble_total;
    char babbles[BABBLE_LIMIT][BABBLE_LENGTH];
};

void add_babble(struct babblespace *ptr, const char *message) {
    pthread_mutex_lock(&(ptr->babble_mutex));
    printf("AAAA\n");

    int index = (ptr->babble_first + ptr->babble_total) % BABBLE_LIMIT;
    printf("AAAA\n");
    strncpy(ptr->babbles[index], message, (BABBLE_LENGTH - 1) + sizeof(USER_NAME));
    ptr->babbles[index][BABBLE_LENGTH - 1] = '\0';

    if (ptr->babble_total < BABBLE_LIMIT) {
        ptr->babble_total++;
    } else {
        ptr->babble_first = (ptr->babble_first + 1) % BABBLE_LIMIT;
    }

    pthread_mutex_unlock(&(ptr->babble_mutex));

}

int main() {
    int shm_fd;
    struct babblespace *shared_memory;

    shm_fd = shm_open(BABBLE_NAME, O_RDWR, BABBLE_MODE);
    shared_memory = (struct babblespace *)mmap(NULL, sizeof(struct babblespace), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    char user_input[BABBLE_LENGTH];
    char message[11 + BABBLE_LENGTH];
    while (strcmp(user_input, "q") != 0) {
        message[0] = '\0'; // Wyczysc tablice przed nadaniem nowego komunikatu
        system("clear");
        for (int i = 0; i < shared_memory->babble_total; ++i) {
        printf("[%d] %s\n", i, shared_memory->babbles[i]);
        }
        printf("Wpisz wiadomosc badz q aby wyjsc ");
        scanf("%s",user_input);
        
        strcat(message, USER_NAME);
        strcat(message, user_input);

        if (strcmp(user_input, "q") != 0) {
            printf("AAAA\n");

            add_babble(shared_memory, message);
        }
    }
	
    system("clear");
    for (int i = 0; i < shared_memory->babble_total; ++i) {
        printf("[%d] %s\n", i, shared_memory->babbles[i]);
    }

    close(shm_fd);

    return 0;
}
