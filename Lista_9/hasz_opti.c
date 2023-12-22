#include  	<openssl/evp.h> //-lssl -lcrypto -pthread
#include  	<string.h>
#include <math.h> // -lm flag
#include <ctype.h> 
// gcc -Wall -pedantic hasz.c -o hasz -lssl -lcrypto -pthread -lm
#define BUFSIZE 128
#define PASS_NUM 182 // Number of passwords to collect

int WORD_NUM = 0;
char PASSWORDS[PASS_NUM][33];
char IS_PASSWORD_GUESSED[PASS_NUM];
int PASSWORD_GUESSED;

pthread_mutex_t mutex;
pthread_cond_t cond;

void bytes2md5(const char *data, int len, char *md5buf) {
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}

void check_all_dir_passwords(char * word){
    for (int z =0; z < PASS_NUM; z++){
        if(IS_PASSWORD_GUESSED[z] == 0 ){
            char md5[33]; // 32 characters + null terminator
            bytes2md5(word, strlen(word), md5);
            if (!strcmp(md5, PASSWORDS[z])){
                printf("HASLO nr. %d, w md5 jest to %s, zostalo zlamane jest to \"%s\" w md5 jest to ", z, PASSWORDS[z], word);
                IS_PASSWORD_GUESSED[z] = 1;
                char md5[33]; // 32 characters + null terminator
                bytes2md5(word, strlen(word), md5);
                printf("%s\n", md5);
            }
        }
    }
}

// NUMBER PREFIXES----------------------------------------------------------------------------------------------------------------

char *number_prefix_full(int front_number, int back_number, const char *str) {
    int front_length = (front_number >= 10) ? 2 : 1;
    int back_length = (back_number >= 10) ? 2 : 1;

    char *front_string = (char *)malloc(front_length + 1);
    snprintf(front_string, front_length + 1, "%d", front_number);

    char *back_string = (char *)malloc(back_length + 1);
    snprintf(back_string, back_length + 1, "%d", back_number);

    int string_length = strlen(str);

    int result_length = string_length + front_length + back_length;
    char *result = (char *)malloc(result_length + 1);

    for (int i = 0; i < front_length; i++) {
        result[i] = front_string[i];
    }

    for (int i = 0; i < string_length; i++) {
        result[i + front_length] = str[i];
    }

    for (int i = 0; i < back_length; i++) {
        result[i + front_length + string_length] = back_string[i];
    }

    result[result_length] = '\0';

    free(front_string);
    free(back_string);

    return result;
}

char *number_prefix_front(int front_number, const char *str) {
    int front_length = (front_number >= 10) ? 2 : 1;
    char *front_string = (char *)malloc(front_length + 1);
    snprintf(front_string, front_length + 1, "%d", front_number);

    int string_length = strlen(str);

    int result_length = string_length + front_length;
    char *result = (char *)malloc(result_length + 1);

    for (int i = 0; i < front_length; i++) {
        result[i] = front_string[i];
    }

    for (int i = 0; i < string_length; i++) {
        result[i + front_length] = str[i];
    }

    result[result_length] = '\0';

    free(front_string);
    return result;
}

char *number_prefix_back(int back_number, const char *str) {
    int back_length = (back_number >= 10) ? 2 : 1;

    char *back_string = (char *)malloc(back_length + 1);
    snprintf(back_string, back_length + 1, "%d", back_number);

    int string_length = strlen(str);

    int result_length = string_length + back_length;
    char *result = (char *)malloc(result_length + 1);

    for (int i = 0; i < string_length; i++) {
        result[i] = str[i];
    }

    for (int i = 0; i < back_length; i++) {
        result[i + string_length] = back_string[i];
    }

    result[result_length] = '\0';

    free(back_string);

    return result;
}

// CAPITALIZATION PREFIXES------------------------------------------------------------------------------------------------------

char *cap_prefix_full(const char *str){
    char *result = (char *)malloc(strlen(str) + 1);

    for (int i = 0; i < strlen(str); i++){
        result[i] = toupper(str[i]);
    }

    result[strlen(str)] = '\0';

    return result;


}

char *cap_prefix_first(const char *str){
    char *result = (char *)malloc(strlen(str) + 1);

    for (int i = 0; i < strlen(str); i++){
        if (i == 0){result[i] = toupper(str[i]);}
        else{result[i] = str[i];}
    }

    result[strlen(str)] = '\0';

    return result;
}
// ADDDING TWO WORDS------------------------------------------------------------------------------------------------------

char * add_words(const char *str1, const char *str2){
    char *result = (char *)malloc(strlen(str1) + strlen(str2) + 2); // \0 sign and ' '
    for (int i = 0; i < strlen(str1); i++){
        result[i] = str1[i];
    }

    result[strlen(str1)] = ' ';
    
    for (int i = 0; i < strlen(str2); i++){
        result[strlen(str1) + 1 + i] = str2[i];
    }

    result[strlen(str1) + strlen(str2)+1] = '\0';

    return result;
}

// GETERS------------------------------------------------------------------------------------------------------

int get_number_of_lines(char * filename){
    FILE *fp = fopen(filename, "r");
    char c;
    int count = 0;
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
 
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') 
            count = count + 1;

    fclose(fp);

    return count;
}

void get_passwords(char * nameOfTheFile){ //
    FILE* hasla = fopen(nameOfTheFile, "r");
    char buf[BUFSIZE];

    for (int i = 0; i < PASS_NUM; i++){
        fgets(buf, BUFSIZE, hasla);
        
        // GET PASSWORD HASHED
        for (int j = 5; j<= 33 + 5; j++){  // 5 first letters are id then we have a password that is 32 letters
            if ((buf[j] != 9) && (buf[j] != 32)){    // Space = 32, TAB = 9
                PASSWORDS[i][j-5] = buf[j];
            }
        }
        PASSWORDS[i][32] = '\0';

        printf("NEW PASWORD: %s\n", PASSWORDS[i]);
    }

    fclose(hasla);

}


// PTHREAD FUNCTIONS------------------------------------------------------------------------------------------------------
void * basic_break(void * prefix_enable_arg){
    FILE* dir = fopen("dir.txt", "r");
    char word_got[BUFSIZE];

    int j=0;
    char prefix_enable = prefix_enable_arg;

    for (int i=0; i<WORD_NUM; i++){
        // RECIVE WORD FROM DIRECTORY
        fgets(word_got, BUFSIZE, dir);
        j=0;
        for (; j<BUFSIZE; j++){ 
            if (word_got[j] == '\n'){break;}
        }

        char word[j];
        j = 0;
        for (; j<BUFSIZE; j++){ 
            if ((word_got[j] == '\n')||(word_got[j] == '\0')){break;}
            word[j] = word_got[j];
        }

        word[j] = '\0';

        // DEPEND ON PREFIX ENABLE SWITCH MODE
        switch (prefix_enable){
        
        case 'a':{ // FULL PREFIX
            for ( int number_i = 0; number_i <  100; number_i++){
                for ( int number_j = 0; number_j <  100; number_j++){
                    check_all_dir_passwords(number_prefix_full(number_i, number_j, word));
                }
            }
            break;
        }
        
        case 'b':{ // FULL PREFIX FIRST LETTER
            for ( int number_i = 0; number_i <  100; number_i++){
                for ( int number_j = 0; number_j <  100; number_j++){
                    char * word_after_front_prefix_cap_first = number_prefix_full(number_i ,number_j, cap_prefix_first(word));
                    check_all_dir_passwords(word_after_front_prefix_cap_first);    
                    
                }
            }
            break;
        }

        case 'c':{ // FULL PREFIX ALL BIG
            for ( int number_i = 0; number_i <  100; number_i++){
                for ( int number_j = 0; number_j <  100; number_j++){  
                    char * word_after_front_prefix_cap_full = number_prefix_full(number_i,number_j , cap_prefix_full(word));
                    check_all_dir_passwords(word_after_front_prefix_cap_full);   
                }
            }
            break;
        }

        case 'd':{ // FRONT PREFIX
            for ( int number_i = 0; number_i <  100; number_i++){    
                check_all_dir_passwords(number_prefix_front(number_i, word));      
            }
            break;
        }

        case 'e':{ // FRONT PREFIX FIRST LETTER
            for ( int number_i = 0; number_i <  100; number_i++){    
                char * word_after_front_prefix_cap_first = number_prefix_front(number_i, cap_prefix_first(word));
                check_all_dir_passwords(word_after_front_prefix_cap_first);   
            }
            break;
        }

        case 'f':{ // FRONT PREFIX ALL BIG
            for ( int number_i = 0; number_i <  100; number_i++){    
                char * word_after_front_prefix_cap_full = number_prefix_front(number_i, cap_prefix_full(word));
                check_all_dir_passwords(word_after_front_prefix_cap_full);   
            }
            break;
        }

        case 'g':{ // BACK PREFIX
            for ( int number_i = 0; number_i <  100; number_i++){
                check_all_dir_passwords(number_prefix_back(number_i, word));   
            }
            
            break;
        }

        case 'h':{ // BACK PREFIX FIRST LETTER
            for ( int number_i = 0; number_i <  100; number_i++){
                char * word_after_back_prefix_cap_first = number_prefix_back(number_i, cap_prefix_first(word));
                check_all_dir_passwords(word_after_back_prefix_cap_first);   
            }
            
            break;
        }

        case 'i':{ // BACK PREFIX ALL BIG
            for ( int number_i = 0; number_i <  100; number_i++){ 
                char * word_after_back_prefix_cap_full = number_prefix_back(number_i,  cap_prefix_full(word));
                check_all_dir_passwords(word_after_back_prefix_cap_full);   
            }
            
            break;
        }

        default:
            check_all_dir_passwords(cap_prefix_full(word));
            check_all_dir_passwords(cap_prefix_first(word));
            check_all_dir_passwords(word);
            break;
        }
    }
    fclose(dir);
    return NULL;
}


void * two_word_break(){
    FILE* dir_1 = fopen("dir.txt", "r");
    FILE* dir_2;

    for (int i = 0; i < WORD_NUM; i++) { // Dir 1 loop
        char word_got[BUFSIZE];
        fgets(word_got, BUFSIZE, dir_1);
        int l = 0;
        for (; l < BUFSIZE; l++) {
            if (word_got[l] == '\n') {
                break;
            }
        }

        char word_1[l];
        l = 0;
        for (; l < BUFSIZE; l++) {
            if ((word_got[l] == '\n') || (word_got[l] == '\0')) {
                break;
            }
            word_1[l] = word_got[l];
        }

        word_1[l] = '\0';

        dir_2 = fopen("dir.txt", "r");
        for (int j = 0; j < WORD_NUM; j++) { // Dir 2 loop

            fgets(word_got, BUFSIZE, dir_2);
            int k = 0;
            for (; k < BUFSIZE; k++) {
                if (word_got[k] == '\n') {
                    break;
                }
            }

            char word_2[k];
            k = 0;
            for (; k < BUFSIZE; k++) {
                if ((word_got[k] == '\n') || (word_got[k] == '\0')) {
                    break;
                }
                word_2[k] = word_got[k];
            }

            word_2[k] = '\0';

            // Add words
            check_all_dir_passwords(add_words(word_1, word_2));
        }
        fclose(dir_2);
    }
    fclose(dir_1);

    return NULL;
}


void * konsument(){
    pthread_mutex_lock(&mutex);
    int N = 0;
    while (N <PASS_NUM)
    {   
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(){
    get_passwords("hasla.txt");
    WORD_NUM = get_number_of_lines("dir.txt") +1;
    pthread_t lamacze[11];
    pthread_t konsument_a;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    
    pthread_create(&lamacze[0], NULL, basic_break, (void *) 'a');
    pthread_create(&lamacze[1], NULL, basic_break, (void *) 'b');
    pthread_create(&lamacze[2], NULL, basic_break, (void *) 'c');
    pthread_create(&lamacze[3], NULL, basic_break, (void *) 'd');
    pthread_create(&lamacze[4], NULL, basic_break, (void *) 'e');
    pthread_create(&lamacze[5], NULL, basic_break, (void *) 'f');
    pthread_create(&lamacze[6], NULL, basic_break, (void *) 'g');
    pthread_create(&lamacze[7], NULL, basic_break, (void *) 'h');
    pthread_create(&lamacze[8], NULL, basic_break, (void *) 'i');
    pthread_create(&lamacze[9], NULL, basic_break, (void *) 'j');
    pthread_create(&lamacze[10], NULL, two_word_break, NULL);
    pthread_create(&konsument_a, NULL, konsument, NULL);

    for (int numerek; numerek < 11; numerek++){
        pthread_join(lamacze[numerek], NULL);
        printf("Przyszedl\n");
    }

    pthread_join(konsument, NULL);

}
