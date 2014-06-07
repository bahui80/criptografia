#ifndef __SECRET_SHARING_H__
#define __SECRET_SHARING_H__

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "constants.h"
#include "lib.h"
#include "image.h"

int getOutputBytes(BYTE * selectedSecretBytes, BYTE * selectedOutputBytes, int k);
int getBi(char * b_string, int bytes, int from);

#endif