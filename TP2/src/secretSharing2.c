#include "../inc/secretSharing2.h"
int global2 = 0;

int
distributeInOneImage(Image secretImage, Image * shadows, int amountOfBytes, int k, int n) {
	int error = NO_ERROR;
	int i, j, index, col, row, superIndex;
	for (i = 0; i < amountOfBytes; i += k) {
		BYTE * selectedSecretBytes = calloc(sizeof(BYTE), k);
		if (selectedSecretBytes == NULL) {
			return CALLOC_ERROR;
		}
		for (j = 0; j < k; j++) {
			selectedSecretBytes[j] = getImage(secretImage)[i + j];
		}
		int ** mat = calloc(sizeof(int * ), k + 1);
		if (mat == NULL) {
			free(selectedSecretBytes);
			return CALLOC_ERROR;
		}
		for (j = 0; j < k + 1; j++) {
			mat[j] = calloc(sizeof(int), n);
			if (mat[j] == NULL) {
				free(selectedSecretBytes);
				free(mat);
				return CALLOC_ERROR;
			}
		}
		for (index = 0; index < n; index++) {
			BYTE * selectedOutputBytes = calloc(sizeof(BYTE), k);
			if (selectedOutputBytes == NULL) {
				return CALLOC_ERROR;
			}
			for (j = 0; j < k; j++) {
				selectedOutputBytes[j] = getImage(shadows[index])[i + j];
			}
			int * values = getOutputBytes(selectedSecretBytes, selectedOutputBytes, k, &error);
			if (error != NO_ERROR) {
				return error;
			}
			for (col = 0; col < k + 1; col++) {
				if (col < k && values[col] == 0) {
					values[col]++;
				}
				// printf("col_ %d\n", col);
				mat[col][index] = values[col];
			}
			// if (global2 < 3) {
			// printf("Mat\n");
			// 	for (row = 0; row < k + 1; row++) {
			// 		for (col = 0; col < n; col++) {
			// 			printf("%d -", mat[row][col]);
			// 		}
			// 		printf("\n");
			// 	}
			// 	global2++;
			// } else {
			// 	exit(1);
			// }
			// exit(1);
		}
		// printf("Mat\n");
		// for (row = 0; row < n; row++) {
		// 	for (col = 0; col < k + 1; col++) {
		// 		printf("%d -", mat[row][col]);
		// 	}
		// 	printf("\n");
		// }
		int auxRow = 0, auxCol = 0;
		int prevRow = 0, prevCol = 0;
		int rta;
		int number = 1;
		int preNumber;
		do {
			
			rta = detVertical3x3(mat);
			
			if (rta == 0) {
				
				do {
					preNumber = mat[auxRow][auxCol];
					if (mat[auxRow][auxCol] > 27) {
						mat[auxRow][auxCol] += number;	
						// printf("Value: %d\n", mat[auxRow][auxCol]);
					} else {
						mat[auxRow][auxCol] -= number;	
					}
					int auxB = 0;
					for (superIndex = 0; superIndex < k; superIndex++) {
						auxB += (mat[superIndex][auxCol] * selectedSecretBytes[superIndex]);
					}
					mat[k][auxCol] = auxB;
					prevCol = auxCol;
					prevRow = auxRow;
					if (auxCol == n - 1) {
						if (auxRow == k - 1) {
							number++;
							auxRow = 0;
						} else {
							auxRow++;
						}
						auxCol = 0;
					} else {
						auxCol++;
					}
					rta = detVertical3x3(mat);

					// if (global2 < 20 ) {
					// 	printf("%d\n", rta);
					// 	global2++;
					// } else {
					// 	global2 = 0;
					// }
					// printf("number %d\n", number);
					// printf("rta = %d\n", rta);
					// printf("Mat\n");
					// for (row = 0; row < k + 1; row++) {
					// 	for (col = 0; col < n; col++) {
					// 		printf("%d -", mat[row][col]);
					// 	}
					// 	printf("\n");
					// }
					// if (rta == 0) {
					// 	mat[prevRow][prevCol] = preNumber;
					// }
				} while (rta == 0);
				// printf("Number: %d\n", number);
			}
		} while (rta == 0);
		for (j = 0; j < n; j++) {
			int * auxiliar = calloc(sizeof(int), k + 1);
			for (index = 0; index < k + 1; index++) {
				auxiliar[index] = mat[index][j];
			}
			int * newValues = calculateOutputValues(auxiliar, &error, k);
			for (col = 0; col < k; col++) {
				setImageInIndex(shadows[j], newValues[col], i + col);
			}
		}

		// if (flag == 1) {
		// 	return NO_ERROR;
		// }

		// return NO_ERROR;
	}
	return NO_ERROR;
}

int *
getOutputBytes(BYTE * selectedSecretBytes, BYTE * selectedOutputBytes, int k, int * error) {
	int lastBytes, bBytes, i;
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
		*error = CALLOC_ERROR;
		return NULL;
	}
	int * values = calloc(sizeof(int), k + 1);
	for (i = 0; i < k; i++) {
		auxAmount = amountOfAi;
		auxAmountBytes = bBytes;
		if (i == k -1) {
			auxAmount = amountOfLastAi;
			auxAmountBytes = lastBytes + 1;
		}
		andB[i] = ((int) selectedOutputBytes[i]) & auxAmount;
		values[i] = (andB[i] >> auxAmountBytes);
		ai = (andB[i] >> auxAmountBytes) * selectedSecretBytes[i]; //TODO: Check if ai = 0 or greater than 251
		a += ai;
	}
	b = a % 251;
	values[i] = b;
	return values;
}

int *
calculateOutputValues(int * values, int * error, int k) {
	int lastBytes, bBytes, i;
	int p = 0;

	if (EIGHT_BITS % k == 0) {
		lastBytes = EIGHT_BITS / k;
		bBytes = lastBytes;
	} else {
		lastBytes = EIGHT_BITS % k;
		bBytes = k;
	}

	p = pow(2, lastBytes);

	char * b_string = byte_to_binary(values[k]);
	if (b_string == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	// printf("b_string: %s\n", b_string);
	int length = strlen(b_string);
	int * auxSelectedOutputBytes = calloc(sizeof(int), k);
	if (auxSelectedOutputBytes == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	for (i = 0; i < k - 1; i++) {
		// if (values[i] == 0) {
		// 	values[i]++;
		// }
		auxSelectedOutputBytes[i] = (values[i] << bBytes) + getBi(b_string, bBytes, bBytes * i);
		if (auxSelectedOutputBytes[i] >= 251) {
			// printf("entra por aca\n");
			auxSelectedOutputBytes[i] = 250;
		}
		// if ((values[i] << bBytes) == 0) {
		// 	printf("values:%d\n", values[i], i);
		// }
		// if (auxSelectedOutputBytes[i] == 0) {
		// 	printf("0\n");
		// }
		// printf("auxSelectedOutputBytes[i] = %d\n", auxSelectedOutputBytes[i]);
	}
	auxSelectedOutputBytes[k - 1] = (values[i] << (lastBytes + 1)) + getBi(b_string, lastBytes, i * bBytes);
	if (auxSelectedOutputBytes[i] >= 251) {
		auxSelectedOutputBytes[i] = 250;
	}
	// printf("auxSelectedOutputBytes[k - 1] = %d\n", auxSelectedOutputBytes[k - 1]);
	// exit(0);

	char * valueForHast = calloc(sizeof(char), (EIGHT_BITS * k) + 1);
	for (i = 0; i < k - 1; i++) {
		strcat(valueForHast, byte_to_binary(auxSelectedOutputBytes[i]));
	}
	char * z = byte_to_binary(auxSelectedOutputBytes[k-1]);
	strncat(valueForHast, z, EIGHT_BITS - lastBytes - 1);
	strcat(valueForHast, z + EIGHT_BITS - lastBytes);
	strcat(valueForHast, "0");
	// printf("valueForHast = %s\n", valueForHast);

 	unsigned char * md = malloc(MD5_DIGEST_LENGTH); 
 	if (md == NULL) {
 		*error = MALLOC_ERROR;
 		return NULL;
 	}
 	unsigned char * hash = MD5(valueForHast, strlen(valueForHast), md);

	int newP = xorFromHashWrapper(hash, error, length);
	if (*error != NO_ERROR) {
		return NULL;
	}
	auxSelectedOutputBytes[k - 1] = auxSelectedOutputBytes[k - 1] + (p * newP);
	return auxSelectedOutputBytes;
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
	// printf("hashInBits = %s\n", hashInBits);
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