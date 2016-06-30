#ifndef AES_BASE64_H
#define AES_BASE64_H

#include <stdio.h>
#include <string.h>  
#include <openssl/aes.h>  
#include <openssl/rand.h>  
#include <openssl/pem.h>


int base64_encode(char *str,int str_len,char *encode, u_int encode_len);
int base64_decode(char *str,int str_len,char *decode,int decode_buffer_len);

int act_code_encrypt_encode(char *act_code, u_int len, char *encode_buf, u_int encode_buf_len);
int act_code_decrypt_decode(char *encode, u_int encode_len, char *act_code, u_int act_code_len);

int aes_encrypte(const u_char *plaintext, u_char *ciphertext, u_int len);

#endif
