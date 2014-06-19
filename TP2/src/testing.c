#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

char *byte_to_binary(int x);

int
main(int argc, char ** argv) {
	int lastBytes;
	int amountOfLastBi = 0;
	int amountOfBi = 0;
	int amountOfAi = 255;
	int amountOfLastAi = 255;
	int p = 0;
	int k = 2;
	int bBytes;
	int i;
	if (8 % k == 0) {
		lastBytes = 8 / k;
		bBytes = lastBytes;
	} else {
		lastBytes = 8 % k;
		bBytes = k;
	}
	for (i = 0; i < lastBytes; i++) {
		amountOfLastBi += pow(2, i);
	}
	for (i = 0; i < bBytes; i++) {
		amountOfBi += pow(2, i);
	}
	amountOfAi -= amountOfBi;
	p = pow(2, lastBytes);
	amountOfLastAi -= (p + amountOfLastBi);
	int andB = 99 & amountOfLastAi;
	andB = andB >> (lastBytes + 1);

	printf("lastBi = %s bi = %s\n", byte_to_binary(amountOfLastBi), byte_to_binary(amountOfBi));
	printf("lastAi = %s ai = %s\n", byte_to_binary(amountOfLastAi), byte_to_binary(amountOfAi));
	printf("p = %s\n", byte_to_binary(p));
}

char *byte_to_binary(int x)
{
    char * b = calloc(sizeof(char), 9);
    if (b == NULL) {
    	return NULL;
    }
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}