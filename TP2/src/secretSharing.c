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

	for (i = 0; i < k - 1; i++) {
		selectedOutputBytes[i] = (char) (andB[i] + getBi(b_string, bBytes, bBytes * i));
	}
	selectedOutputBytes[k - 1] = (char) (andB[k - 1] + getBi(b_string, lastBytes, i * bBytes) + p);

	return error;
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