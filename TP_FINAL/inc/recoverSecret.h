#ifndef __RECOVER_SECRET_H__
#define __RECOVER_SECRET_H__

#include "constants.h"
#include "main.h"
#include <math.h>
#include "lib.h"

int * valuesFork3(int ** mat, int k, int * error);
Image recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error);
int * valuesFork2(int ** mat, int k, int * error);
void checkPValue(int * values, int k, int * error);

#endif
