#ifndef __PROVISORIO_H__
#define __PROVISORIO_H__

#include "constants.h"
#include "main.h"
#include <math.h>
#include "lib.h"

int * valuesFork3(int ** mat, int k, int * error);
Image recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error);
int * valuesFork2(int ** mat, int k, int * error);

#endif