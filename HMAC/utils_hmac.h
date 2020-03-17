
#ifndef UTILS_HMAC_H_
#define UTILS_HMAC_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

void utils_hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len);
void utils_hmac_sha1(const char *msg, int msg_len, char *digest, const char *key, int key_len);
int base64_decode( const char * base64, unsigned char * bindata );

#endif

