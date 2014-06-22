#include "../inc/secretSharing2.h"

int
distributeInOneImage(Image secretImage, Image * shadows, int amountOfBytes, int k, int n) {
	int error = NO_ERROR;
	int i, j, index, col, row;
	for (i = 0; i < amountOfBytes; i += k) {
		BYTE * selectedSecretBytes = calloc(sizeof(BYTE), k);
		if (selectedSecretBytes == NULL) {
			return CALLOC_ERROR;
		}
		for (j = 0; j < k; j++) {
			selectedSecretBytes[j] = getImage(secretImage)[i + j];
		}
		int ** mat = calloc(sizeof(int * ), n);
		if (mat == NULL) {
			free(selectedSecretBytes);
			return CALLOC_ERROR;
		}
		for (j = 0; j < n; j++) {
			mat[j] = calloc(sizeof(int), k + 1);
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
				mat[index][col] = values[col];
			}
		}
		if (k == 3) {
			checkLiForK3(mat, k, n);
		} else if (k == 2) {
			checkLiForK2(mat, k, n);
		}
		
		for (j = 0; j < n; j++) {
			int s, auxiliarB = 0;
			for (s = 0; s < k; s++) {
				auxiliarB += selectedSecretBytes[s] * mat[j][s];
			}
			mat[j][k] = auxiliarB % 251; 
			int * newValues = calculateOutputValues(mat[j], &error, k);
			for (col = 0; col < k; col++) {
				setImageInIndex(shadows[j], newValues[col], i + col);
			}
		}
	}
	return NO_ERROR;
}

void
checkLiForK2(int ** matrix, int k, int n) {
	int i, j, matIndex1, matIndex2;
   	i = 0;
   	while (i < n) {
   		j = 0;
   		while(j < n) {
   			if (j == i) {
   				++j;
   			} else {
   				int ** mat = calloc(sizeof(int), k);
   				for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
   					mat[matIndex1] = calloc(sizeof(int), k);
   				}
   				for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
   					mat[0][matIndex1] = matrix[i][matIndex1];
   					mat[1][matIndex1] = matrix[j][matIndex1];
   				}
   				int rta;
              int auxRow = 0, auxCol = 0;
              int count = 0;
              do {
                rta = det2x2(mat);
                if (rta == 0) {
                	count++;
                	if (k == 3) {
						if (mat[auxRow][auxCol] == 7) {
							mat[auxRow][auxCol] -= 2;
						}
					} 
					mat[auxRow][auxCol]++;
					int auxB = 0;			
					if (auxCol == 1) {
						if (auxRow == 1) {
							auxRow = 0;
							auxCol = 0;
						} else {
							auxRow++;
						}
					} else {
						auxCol++;
					}
                }
              } while (rta == 0 && count < 100);
   				for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
   					matrix[i][matIndex1] = mat[0][matIndex1];
   					matrix[j][matIndex1] = mat[1][matIndex1];
   				}
   				j++;
   			}
   		}
   		i++;
   	}
}

void
checkLiForK3(int ** matrix, int k, int n) {
	int i, j, l, matIndex1, matIndex2;
    i = 0;
    while (i < n) {
      j = 0;
      while(j < n) {
        if (j == i) {
          ++j;
        } else {
          l = 0;
          while (l < n) {
            if (l == i || l == j) {
              ++l;
            } else {
              int ** mat = calloc(sizeof(int), k + 2);
              for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
                mat[matIndex1] = calloc(sizeof(int), k + 2);
              }
              for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
                mat[0][matIndex1] = matrix[i][matIndex1];
                mat[1][matIndex1] = matrix[j][matIndex1];
                mat[2][matIndex1] = matrix[l][matIndex1];
              }
              int rta;
              int auxRow = 0, auxCol = 0;
              int count = 0;
              do {
                rta = detVertical3x3(mat);
                if (rta == 0) {
                	count++;
                	if (k == 3) {
						if (mat[auxRow][auxCol] == 31) {
							mat[auxRow][auxCol] -= 4;
						}
					} 
					mat[auxRow][auxCol]++;
					int auxB = 0;			
					if (auxCol == 2) {
						if (auxRow == 2) {
							auxRow = 0;
							auxCol = 0;
						} else {
							auxRow++;
						}
					} else {
						auxCol++;
					}
                }
              } while (rta == 0 && count < 100);
              for(matIndex1 = 0; matIndex1 < k; matIndex1++) {
                matrix[i][matIndex1] = mat[0][matIndex1];
                matrix[j][matIndex1] = mat[1][matIndex1];
                matrix[l][matIndex1] = mat[2][matIndex1];
              }
              free(mat);
              l++;
            }
          }
          j++;
        }
      }
      i++;
    }
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
		ai = (andB[i] >> auxAmountBytes) * selectedSecretBytes[i];
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
	int length = strlen(b_string);
	int * auxSelectedOutputBytes = calloc(sizeof(int), k);
	if (auxSelectedOutputBytes == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	for (i = 0; i < k - 1; i++) {
		auxSelectedOutputBytes[i] = (values[i] << bBytes) + getBi(b_string, bBytes, bBytes * i);
	}
	auxSelectedOutputBytes[k - 1] = (values[i] << (lastBytes + 1)) + getBi(b_string, lastBytes, i * bBytes);

	char * valueForHast = calloc(sizeof(char), (EIGHT_BITS * k) + 1);
	for (i = 0; i < k - 1; i++) {
		strcat(valueForHast, byte_to_binary(auxSelectedOutputBytes[i]));
	}
	char * z = byte_to_binary(auxSelectedOutputBytes[k-1]);
	strncat(valueForHast, z, EIGHT_BITS - lastBytes - 1);
	strcat(valueForHast, z + EIGHT_BITS - lastBytes);
	strcat(valueForHast, "0");

 	unsigned char * md = malloc(MD5_DIGEST_LENGTH); 
 	if (md == NULL) {
 		*error = MALLOC_ERROR;
 		return NULL;
 	}
 	unsigned char * hash = MD5(valueForHast, strlen(valueForHast), md);

	int newP = xorFromHashWrapper(hash, error, length);
	if (newP != 0) {
		printf("newP: %d\n", newP);
	}
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

int
getRandom(int k) {
  srand(time(NULL));
  int randomnumber;
    return rand() % k;
}