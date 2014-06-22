#ifndef __SECRET_SHARING_H__
#define __SECRET_SHARING_H__

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <openssl/md5.h>
#include "constants.h"
#include "lib.h"
#include "image.h"
#include <time.h>

int distributeInOneImage(Image secretImage, Image * shadows, int amountOfBytes, int k, int n);
int * getOutputBytes(BYTE * selectedSecretBytes, BYTE * selectedOutputBytes, int k, int * error);
int getBi(char * b_string, int bytes, int from);
int xorFromHashWrapper(unsigned char * hash, int * error, int length);
int xorFromHash(char * hash);
int * calculateOutputValues(int * values, int * error, int k);
void checkLiForK3(int ** matrix, int k, int n);
int getRandom(int k);
void checkLiForK2(int ** matrix, int k, int n);

#endif