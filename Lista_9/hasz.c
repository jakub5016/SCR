#include  	<openssl/evp.h> //-lssl -lcrypto -pthread
#include  	<string.h>
#include <math.h> // -lm flag
#include <ctype.h> 

#define BUFSIZE 128
#define PASS_NUM 21 // Number of passwords to collect

int WORD_NUM = 0;
char PASSWORDS[PASS_NUM][33];


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

int check_pass(char * pass, char * correct_pass){
    char md5[33]; // 32 characters + null terminator
    bytes2md5(pass, strlen(pass), md5);
    int isTheSame = 1;

    for (int i= 0; i<33; i++){
        if (correct_pass[i] != md5[i]){
            isTheSame = 0;
            break;
        }
    }
    return isTheSame;
}

void check_all_dir_passwords(char * word){
    for (int z =0; z < PASS_NUM; z++){
            if (check_pass(word, PASSWORDS[z])){
                printf("HASLO nr. %d, w md5 jest to %s, zostalo zlamane jest to \"%s\" w md5 jest to ", z, PASSWORDS[z], word);
                char md5[33]; // 32 characters + null terminator
                bytes2md5(word, strlen(word), md5);
                printf("%s\n", md5);

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

void * basic_break(void * prefix_enable_arg){
    FILE* dir = fopen("dir.txt", "r");
    char word_got[BUFSIZE];

    int j=0;
    int prefix_enable = prefix_enable_arg;

    for (int i=0; i<WORD_NUM; i++){
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

        if (prefix_enable > 0){ //Number prefix gives us info about how mutch numbers we can get at the beginning and end

            // FULL NUMBER PREFIX 
            for ( int number_i = 0; number_i <  100; number_i++){
                for ( int number_j = 0; number_j <  100; number_j++){
                    
                    char * word_after_full_prefix = number_prefix_full(number_i, number_j, word);
                    check_all_dir_passwords(word_after_full_prefix);   
                    if (prefix_enable > 1){
                        char * word_after_cap_full = cap_prefix_full(word);
                        char * word_after_cap_first = cap_prefix_first(word);

                        char * word_after_front_prefix_cap_first = number_prefix_full(number_i ,number_j, word_after_cap_first);
                        check_all_dir_passwords(word_after_front_prefix_cap_first);   
                        char * word_after_front_prefix_cap_full = number_prefix_full(number_i,number_j ,word_after_cap_full);
                        check_all_dir_passwords(word_after_front_prefix_cap_full);   
                    }
                }
            }

            // FRONT NUMBER PREFIX
            for ( int number_i = 0; number_i <  100; number_i++){    
                // Basic ver            
                char * word_after_front_prefix = number_prefix_front(number_i, word);
                check_all_dir_passwords(word_after_front_prefix);   
                
                //Adding caps
                if (prefix_enable > 1){
                    char * word_after_cap_full = cap_prefix_full(word);
                    char * word_after_cap_first = cap_prefix_first(word);

                    char * word_after_front_prefix_cap_first = number_prefix_front(number_i, word_after_cap_first);
                    check_all_dir_passwords(word_after_front_prefix_cap_first);   
                    char * word_after_front_prefix_cap_full = number_prefix_front(number_i, word_after_cap_full);
                    check_all_dir_passwords(word_after_front_prefix_cap_full);   
                }
            }
            // BACK NUMBER PREFIX
            for ( int number_i = 0; number_i <  100; number_i++){
                char * word_after_back_prefix = number_prefix_back(number_i, word);
                check_all_dir_passwords(word_after_back_prefix);   
                if (prefix_enable > 1){
                    char * word_after_cap_full = cap_prefix_full(word);
                    char * word_after_cap_first = cap_prefix_first(word);

                    char * word_after_back_prefix_cap_first = number_prefix_back(number_i, word_after_cap_first);
                    check_all_dir_passwords(word_after_back_prefix_cap_first);   
                    char * word_after_back_prefix_cap_full = number_prefix_back(number_i, word_after_cap_full);
                    check_all_dir_passwords(word_after_back_prefix_cap_full);   
                }
            }
        } 
        else{
            if (prefix_enable > -1){
                check_all_dir_passwords(cap_prefix_full(word));
                check_all_dir_passwords(cap_prefix_first(word));
            }
            check_all_dir_passwords(word);
        }
    }
    fclose(dir);

    return NULL;

}

int main(){
    get_passwords("hasla.txt");
    WORD_NUM = get_number_of_lines("dir.txt") +1;
    basic_break((void *)2);
    // pthread_t lamacze[3];

    // pthread_create(&lamacze[0], NULL, basic_break, (void *) 0);
    // pthread_create(&lamacze[1], NULL, basic_break, (void *) 1);
    // pthread_create(&lamacze[2], NULL, basic_break, (void *) 2);

    // pthread_join(&lamacze[0], NULL);
    // printf("Przyszedl\n");
    // pthread_join(&lamacze[1], NULL);
    // printf("Przyszedl\n");
    // pthread_join(&lamacze[2], NULL);
    // printf("Przyszedl\n");


    // char * slowo = "Slowo";

    // printf("%s\n", slowo);

    // char * zmodyfikowane_slowo = number_prefix(2, 2, slowo);

    // printf("%s\n", zmodyfikowane_slowo);
}
