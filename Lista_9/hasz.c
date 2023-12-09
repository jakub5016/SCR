#include  	<openssl/evp.h>
#include  	<string.h>
#define BUFSIZE 128
#define PASS_NUM 10 // Number of passwords to collect

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

    // const char *test = "Wrong99";
    // char md5[33]; // 32 characters + null terminator
    // bytes2md5(test, strlen(test), md5);
    // printf("%s ======================> %s\n", test, md5);
}



int main(){

    FILE* hasla = fopen("hasla.txt", "r");
    FILE* dir = fopen("dir.txt", "r");

    char buf[BUFSIZE];
    char passwords[PASS_NUM][32];

    // Space = 32 in asci
    for (int i = 0; i < PASS_NUM; i++){
        fgets(buf, BUFSIZE, hasla);
        
        // GET PASSWORD HASHED
        for (int j = 3; j<= 32 + 4; j++){  // 3 first letters are id then we have a password that is 32 letters
            if ((buf[j] != 9) && (buf[j] != 32)){    // Space = 32, TAB = 9
                passwords[i][j-5] = buf[j];
            }
        }
    }
    // Show passwords
    for (int i=0; i<PASS_NUM; i++){ \
        printf("PASSWORD[%d] : ", i);
       
        for (int j = 0; j< 32; j++){
            printf("%c",passwords[i][j]);
        }
        printf("\n");
    }
}
