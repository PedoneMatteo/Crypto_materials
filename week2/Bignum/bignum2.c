#include <stdio.h>
#include <openssl/bn.h>

int main ()
{
  char num_string[] = "123456789012345678901234567890123456789012345678901234567890";
  char hex_string[] = "13AAF504E4BC1E62173F87A4378C37B49C8CCFF196CE3F0AD2";

  BIGNUM *big_number = BN_new();
  //creeate from decimal string
  BN_dec2bn(&big_number, num_string);   //TRANSFORMATION FROM DECIMAL TO BINARY: the first parameter is the destination and the second one is the initial array
  BIGNUM *big_number2 = BN_new();
  BN_hex2bn(&big_number2, hex_string);  //TRANSFORMATION FROM HEXADECIMAL TO BINARY: the first parameter is the destination and the second one is the initial array

  BN_print_fp(stdout,big_number);
  printf("\n");
  BN_print_fp(stdout,big_number2);
  printf("\n");

  int res = BN_cmp(big_number2, big_number);    //COMPARISON BETWEEN TWUO BIG NUMBERS
  if(res==0)
    printf("The big numbers are equal\n");
  else
    printf("The big numbers are different\n");

  char *num_string_after = BN_bn2hex(big_number);   //TRANSFORMATIION FROM BINARY TO HEXADECIMAL
  printf("%s\n", num_string_after);
  printf("%s\n",  BN_bn2dec(big_number));   //TRANSFORMATIION FROM BINARY TO DECIMAL

  // politely free OpenSSL generated heap structures
  OPENSSL_free(num_string_after);
  BN_free(big_number);

  return 0;
}