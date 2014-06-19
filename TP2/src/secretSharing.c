#include "../inc/secretSharing.h"

int
distributeInOneImage(Image secretImage, Image outputImage, int amountOfBytes, int k) {
	int error = NO_ERROR;
	int i, j;
	for (i = 0; i < amountOfBytes; i += k) {
		BYTE * selectedSecretBytes = calloc(sizeof(BYTE), k);
		if (selectedSecretBytes == NULL) {
			return CALLOC_ERROR;
		}
		for (j = 0; j < k; j++) {
			selectedSecretBytes[j] = getImage(secretImage)[i + j];
		}
		BYTE * selectedOutputBytes = calloc(sizeof(BYTE), k);
		if (selectedOutputBytes == NULL) {
			return CALLOC_ERROR;
		}
		for (j = 0; j < k; j++) {
			selectedSecretBytes[j] = getImage(outputImage)[i + j];
		}
		error = getOutputBytes(selectedSecretBytes, selectedOutputBytes, k);
		if (error != NO_ERROR) {
			free(selectedSecretBytes);
			free(selectedOutputBytes);
			return error;
		}
		for (j = 0; j < k; j++) {
			setImageInIndex(outputImage, selectedOutputBytes[j], i + j);
		}
	}
	return error;
}

int
getOutputBytes(BYTE * selectedSecretBytes, BYTE * selectedOutputBytes, int k) {
	int lastBytes, bBytes, i, error = NO_ERROR;
	int amountOfLastBi = 0, amountOfBi = 0, p = 0, a = 0;
	int amountOfAi = 255, amountOfLastAi = 255;
	int * andB, auxAmount, auxAmountBytes, b, ai;

	if (EIGHT_BITS % k == 0) {
		lastBytes = EIGHT_BITS / k;
		bBytes = lastBytes;
	} else {
		lastBytes = EIGHT_BITS % k;
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
	andB = calloc(sizeof(int), k);
	if (andB == NULL) {
		return CALLOC_ERROR;
	}
	for (i = 0; i < k; i++) {
		auxAmount = amountOfAi;
		auxAmountBytes = bBytes;
		if (i == k -1) {
			auxAmount = amountOfLastAi;
			auxAmountBytes = lastBytes + 1;
		}
		andB[i] = ((int) selectedOutputBytes[i]) & auxAmount;
		ai = (andB[i] >> auxAmountBytes) * selectedSecretBytes[i]; //TODO: Check if ai = 0 or greater than 251
		a += ai;
	}
	b = a % 251;
	char * b_string = byte_to_binary(b);
	if (b_string == NULL) {
		return CALLOC_ERROR;
	}
	int length = strlen(b_string);
	int * auxSelectedOutputBytes = calloc(sizeof(int), k);
	if (auxSelectedOutputBytes == NULL) {
		return CALLOC_ERROR;
	}
	for (i = 0; i < k - 1; i++) {
		auxSelectedOutputBytes[i] = andB[i] + getBi(b_string, bBytes, bBytes * i);
		printf("auxSelectedOutputBytes[i] = %d\n", auxSelectedOutputBytes[i]);
	}
	auxSelectedOutputBytes[k - 1] = andB[k - 1] + getBi(b_string, lastBytes, i * bBytes);
	printf("auxSelectedOutputBytes[k - 1] = %d\n", auxSelectedOutputBytes[k - 1]);

	char * valueForHast = calloc(sizeof(char), (EIGHT_BITS * k) + 1);
	for (i = 0; i < k - 1; i++) {
		strcat(valueForHast, byte_to_binary(auxSelectedOutputBytes[i]));
	}
	char * z = byte_to_binary(auxSelectedOutputBytes[k-1]);
	strncat(valueForHast, z, EIGHT_BITS - lastBytes - 1);
	strcat(valueForHast, z + EIGHT_BITS - lastBytes);
	strcat(valueForHast, "0");
	printf("valueForHast = %s\n", valueForHast);

 	unsigned char * md = malloc(MD5_DIGEST_LENGTH); 
 	if (md == NULL) {
 		return MALLOC_ERROR;
 	}
 	unsigned char * hash = MD5(valueForHast, strlen(valueForHast), md);

	int newP = xorFromHashWrapper(hash, &error, length);
	if (error != NO_ERROR) {
		return error;
	}
	printf("result = %d\n", newP);
	for (i = 0; i < k - 1; i++) {
		selectedOutputBytes[i] = auxSelectedOutputBytes[i];
	}
	selectedOutputBytes[k - 1] = auxSelectedOutputBytes[k - 1] + (p * newP);

 
	return error;
}

int
xorFromHashWrapper(unsigned char * hash, int * error, int length) {
	int i;
	char * hashInBits = calloc(sizeof(char), 128);
	if (hashInBits == NULL) {
		*error = CALLOC_ERROR;
		return -1;
	}

	for (i = 0; i < length; i++) {
		strcat(hashInBits, byte_to_binary((int)hash[i]));
	}
	printf("hashInBits = %s\n", hashInBits);
	return xorFromHash(hashInBits);
}

int
xorFromHash(char * hash) {
	unsigned int result = hash[0] - '0';
	int i;
	int length = strlen(hash);
	for (i = 1; i < length; i++) {
		result = result ^ (hash[i] - '0');
	}
	return result;
}

int
getBi(char * b_string, int bytes, int from) {
	int i, bi;
	int result = 0;
	for (i = 0; i < bytes; i++) {
		bi = (int) (b_string[from + i] - '0');
		result += pow(2, bytes - i - 1) * bi;
	}
	return result;
}