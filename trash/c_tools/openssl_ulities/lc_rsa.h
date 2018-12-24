
#ifndef IRI_RSA_H
#define IRI_RSA_H

#include <stdio.h>   
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define PUB_KEY			1
#define PRIV_KEY		0

int rsa_base64(char *instr, int inlen, char** outstr, int type);
int just_test();

#endif
