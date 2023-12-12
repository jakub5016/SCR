#include  	<openssl/evp.h>
#include  	<string.h>
#define BUFSIZE 128
#define PASS_NUM 5 // Number of passwords to collect
#define WORD_NUM 15

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

int main(){

    FILE* dir = fopen("dir.txt", "r");
    
    get_passwords("hasla.txt");

    // Getting a word from dir and trying to break password
    char word_got[BUFSIZE];

    for (int i; i < BUFSIZE; i++){
        word_got[i] = '\0';
    }

    int j=0;
    
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

        // printf("%s\n" ,word);

        for (int j = 0; j < PASS_NUM; j++){
            if (check_pass(word, PASSWORDS[j])){
                printf("HASLO nr. %d, w md5 jest to %s, zostalo zlamane jest to \"%s\" w md5 jest to ", j, PASSWORDS[j], word);
                char md5[33]; // 32 characters + null terminator
                bytes2md5(word, strlen(word), md5);
                printf("%s\n", md5);

            }
        }
        
    }

}
