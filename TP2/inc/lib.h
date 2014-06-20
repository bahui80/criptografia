#ifndef __LIB_H__
#define __LIB_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"

char * byte_to_binary(int x);
int detVertical3x3(int ** mat);
int detHorizontal3x3(int ** mat);
int calculate_inverse(int b);
int  det2x2(int ** mat);

#endif