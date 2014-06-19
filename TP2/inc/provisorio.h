#ifndef __PROVISORIO_H__
#define __PROVISORIO_H__

#include "constants.h"
#include "main.h"
#include <math.h>
#include "lib.h"

Image recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error);
int detVertical3x3(int ** mat);
int detHorizontal3x3(int ** mat);
int calculate_inverse(int b);

#endif