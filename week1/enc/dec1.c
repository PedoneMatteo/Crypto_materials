#include <stdio.h>      /*SIMPLE DECRYPT*/
#include <string.h>

#include <openssl/evp.h>


#define ENCRYPT 1
#define DECRYPT 0

int main()
{

//int EVP_EncryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv);
//int EVP_EncryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);

//  int EVP_DecryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv);
//  int EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

//  int EVP_CipherInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, const unsigned char *key, const unsigned char *iv, int enc);
//  int EVP_CipherFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);


    unsigned char key[] = "0123456789ABCDEF";
    unsigned char iv[]  = "1111111111111111";
    unsigned char ciphertext_hex[] = "42279afcc3fddda4ebaafaa85c75920246e17cb060ab0567ecce7e740b5f956c403fb9860edd65a23c738be646baf324";

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit(ctx,EVP_aes_128_cbc(), key, iv, DECRYPT);
    
    // convert hexstring into bytes
    int ciphertext_len = strlen(ciphertext_hex)/2;
    printf("Cyphertext_hex len: %d\n",strlen(ciphertext_hex)); //questo da risultato 96 in quanto considera il singolo numero, ma in realtà la vera rappresentazione 
                                                               // è in blocchetti da due numeri, che in esadecimale vanno a identificare un carattere/lettera/byte.
                                                               // Quindi la vera lunghezza è 96/2 = 48 (es. 't'=0x54, di cui in teoria il primo ha lunghezza 1 e il 
                                                               // secondo ha lunghezza 2 ma sono la stessa cosa)
    unsigned char ciphertext_binary[ciphertext_len];

    for(int i = 0; i < ciphertext_len;i++){
        sscanf(&ciphertext_hex[2*i],"%2hhx", &ciphertext_binary[i]);    /*every time that we move of 1 byte, we skip 2 characters into the hexadecimal digits rappresentation*/
    }                              // ^--passing 1 byte at the time
    printf("Cyphertext binary:\n");
    
    for(int i = 0; i < ciphertext_len; i++)
        printf("%02x", ciphertext_binary[i]);
    printf("\n\n");


    unsigned char decrypted[ciphertext_len]; //may be larger than needed due to padding

    int update_len, final_len;
    int decrypted_len=0;

    EVP_CipherUpdate(ctx,decrypted,&update_len,ciphertext_binary,ciphertext_len);
    decrypted_len+=update_len;
    printf("update size: %d\n",decrypted_len);

    EVP_CipherFinal_ex(ctx,decrypted+decrypted_len,&final_len);
    decrypted_len+=final_len;

    EVP_CIPHER_CTX_free(ctx);

    printf("Plaintext lenght = %d + %d = %d\n",update_len, final_len, decrypted_len);
    for(int i = 0; i < decrypted_len; i++)
        printf("%2x", decrypted[i]);
    printf("\n");
    for(int i = 0; i < decrypted_len; i++)
        printf("%c", decrypted[i]);
    printf("\n");

    return 0;
}