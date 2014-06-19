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

int distributeInOneImage(Image secretImage, Image outputImage, int amountOfBytes, int k);
int getOutputBytes(BYTE * selectedSecretBytes, BYTE * selectedOutputBytes, int k);
int getBi(char * b_string, int bytes, int from);
int xorFromHashWrapper(unsigned char * hash, int * error, int length);
int xorFromHash(char * hash);

#endif