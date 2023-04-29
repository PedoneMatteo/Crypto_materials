#include <stdio.h>          /*ENCRYPTION OF DATA THAT ARE STORED IN A FILE*/
#include <string.h>
                //sul terminale: ./enc3 enc3.c 1111111122222222333333334444444455555555666666667777777788888888 1111111122222922233233333444442455555455666666667777877788888888
#include <openssl/evp.h>
#include <openssl/err.h>


#define ENCRYPT 1
#define DECRYPT 0
#define MAX_ENC_LEN 1000000
#define MAX_BUFFER  1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

/*
        argv[1] = input file
        argv[2] = key (it is an hexstring)
        argv[3] = IV  (it is an hexstring)
        argv[4] = file output (optional)
        save in a buffer in memory the result of encryption
*/

int main(int argc, char **argv)     
{

//  int EVP_CipherInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv, int enc);
//  int EVP_CipherUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
//  int EVP_CipherFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

    if(argc != 5){
        fprintf(stderr,"Invalid parameters. Usage: %s filename key iv\n",argv[0]);
        exit(1);
    }

    FILE *f_in;
    if((f_in = fopen(argv[1],"r")) == NULL) {
            fprintf(stderr,"Couldn't open the input file, try again\n");
            abort();
    }

    FILE *f_out;
    if((f_out = fopen(argv[4], "wb")==NULL)){
        fprintf(stderr,"Couldn't open the input file, try again\n");
        abort();
    }

    if(strlen(argv[2])!=32){
        fprintf(stderr,"Wrong key length\n");
        abort();
    }   
    if(strlen(argv[3])!=32){
        fprintf(stderr,"Wrong IV length\n");
        abort();
    }
        

        
    // convert hexstring (key) into bytes
    unsigned char key[strlen(argv[2])/2];
    for(int i = 0; i < strlen(argv[2])/2;i++){
        sscanf(&argv[2][2*i],"%2hhx", &key[i]);
    }
    // convert hexstring (IV) into bytes
    unsigned char iv[strlen(argv[3])/2];
    for(int i = 0; i < strlen(argv[3])/2;i++){
        sscanf(&argv[3][2*i],"%2hhx", &iv[i]);
    }


    /* Load the human readable error strings for libcrypto */
    ERR_load_crypto_strings();
    /* Load all digest and cipher algorithms */
    OpenSSL_add_all_algorithms();

    // pedantic mode: check NULL
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    if(!EVP_CipherInit(ctx,EVP_aes_128_cbc(), key, iv, ENCRYPT))
        handle_errors();

    
    
    unsigned char ciphertext[MAX_BUFFER +16];

    int update_len, final_len;
    int ciphertext_len=0;
    int n_read;
    unsigned char buffer[MAX_BUFFER];


    while((n_read = fread(buffer,1,MAX_BUFFER,f_in)) > 0){
            // if you risk to overflow the buffer variable, you exit before
            // n_read + 1 block > left in cyphertext (MAX - cyphertext_len)
    /*  if(ciphertext_len > MAX_ENC_LEN - n_read - EVP_CIPHER_CTX_block_size(ctx)){ //use EVP_CIPHER_get_block_size with OpenSSL 3.0+
            fprintf(stderr,"The file to cipher is larger than I can\n");
            abort();
        }   */      //this check is useless is the output is in a file but it is necessary if the output is in a variable
    
        if(!EVP_CipherUpdate(ctx, ciphertext /*+ ciphertext_len*/ ,&update_len,buffer,n_read))
            handle_errors();
        ciphertext_len+=update_len;

        if(fwrite(ciphertext, 1, update_len, f_out) < update_len){
            fprintf(stderr, "Error writing into the output file\n");
            abort();
        }
    }

    if(!EVP_CipherFinal_ex(ctx,ciphertext/*+ciphertext_len*/,&final_len))
        handle_errors();

    ciphertext_len+=final_len;

    if(fwrite(ciphertext, 1, update_len, f_out) < update_len){
        fprintf(stderr, "Error writing into the output file\n");
        abort();
    }

    EVP_CIPHER_CTX_free(ctx);

    printf("Ciphertext length = %d\n", ciphertext_len);
    /*for(int i = 0; i < ciphertext_len; i++)
        printf("%02x", ciphertext[i]);
    printf("\n");*/


    // completely free all the cipher data
    CRYPTO_cleanup_all_ex_data();
    /* Remove error strings */
    ERR_free_strings();

    fclose(f_in);
    fclose(f_out);
    
    return 0;
}