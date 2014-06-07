#include "../inc/lib.h"

char *
byte_to_binary(int x) {
    char * b = calloc(sizeof(char), 9);
    if (b == NULL) {
    	return NULL;
    }
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}