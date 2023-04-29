#include <stdio.h>      /*SIMPLE ENCRYPT*/
#include <string.h>

#include <openssl/evp.h>

#define ENCRYPT 1
#define DECRYPT 0

int main()
{
    unsigned char key[] = "0123456789ABCDEF";   //ASCII
    unsigned char iv[]  = "1111111111111111";   //ASCII -> len=16
    
    //CONTEXT for storing cypher objects
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); 



    //INITIALIZATION (parameters)-> context, name of algorithm, key, IV, parameter to report if we are encrypting or decrypting
    EVP_CipherInit(ctx,EVP_aes_128_cbc(), key, iv, ENCRYPT);    

    unsigned char plaintext[] = "This is the plaintext to encrypt."; //len 33 !!! 

    printf("plaintext length: %d\n", strlen(plaintext));

    printf("Initial plaintext: \n");
    for(int j; j<strlen(plaintext); j++)
        printf("%x\t", plaintext[j]);
    printf("\n\n");
    unsigned char ciphertext[48]; //-> length=48 because it is the first multiple of 16 after 33 (length of plaintext) [surely there will be a padding a the end of cyphertext]
                                  //NB: la lunghezza 48 fa riferimento al numero di caratteri/byte, ma ogni carattere in esadecimale corrisponde a una rappresentazione 
                                  //    con due numeri (es. 't' = 0x54). Quindi se considerassimo le lettere la lunghezza è 48, ma se considerassimo i singoli numeri, e non
                                  //    i blocchetti da due numeri, la lunghezza è 48*2=96
    int update_len, final_len;
    int ciphertext_len=0;


    //UPDATE (parameters) -> context, variable in which storing the result (cyphertext), integer in which report the length of cyphertext, plaintext, size of plaintext
    //viene aggiunto il padding
    EVP_CipherUpdate(ctx,ciphertext,&update_len,plaintext,strlen(plaintext));
    ciphertext_len+=update_len;

    printf("update size: %d\nCyphertext:\n",ciphertext_len);
    for(int i = 0; i < ciphertext_len; i++)
        printf("%x\t", ciphertext[i]);
    printf("\n\n");



    //FINALIZATION (parameters)-> context, free address after the fields busy from the cyphertext, variable in which storing the final length
    EVP_CipherFinal_ex(ctx,ciphertext+ciphertext_len,&final_len);
    printf("final len: %d\n",final_len);
    ciphertext_len+=final_len;



    //FREE the object
    EVP_CIPHER_CTX_free(ctx);


    printf("Ciphertext total lenght => %d + %d = %d\n", update_len, final_len, ciphertext_len);
    printf("Cypher text obtained:\n");
    for(int i = 0; i < ciphertext_len; i++)
        printf("%02x", ciphertext[i]);
    printf("\n");

    return 0;
}
