#include "../inc/recoverSecret.h"

int globalP = TRUE;

Image
recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error) {
	int lastBytes, bBytes, i, j;
	int amountOfLastBi = 0, amountOfBi = 0, p = 0;
	int amountOfAi = 255, amountOfLastAi = 255;
	int shadowImagesIndex, index, sizeIndex, auxB;
	int * shadowVector, vectorIndex, * values;
	Image originalImage;
	BYTE * image, * imageAux;

	if (8 % k == 0) {
		lastBytes = 8 / k;
		bBytes = lastBytes;
	} else {
		lastBytes = 8 % k;
		bBytes = k;
	}
	for (j = 0; j < lastBytes; j++) {
		amountOfLastBi += pow(2, j);
	}
	for (j = 0; j < bBytes; j++) {
		amountOfBi += pow(2, j);
	}
	amountOfAi -= amountOfBi;
	p = pow(2, lastBytes);
	amountOfLastAi -= (p + amountOfLastBi);
	originalImage = clone(shadowImages[0], error);
	if (*error != NO_ERROR) {
		return NULL;
	}
	imageAux = calloc(sizeof(BYTE), amountOfBytes);
	if (imageAux == NULL) {
		free(originalImage);
		*error = CALLOC_ERROR;
		return NULL;
	}
	for (sizeIndex = 0; sizeIndex < amountOfBytes; sizeIndex += k) {
		int ** mat = calloc(sizeof(int *), k);
		if (mat == NULL) {
			free(originalImage);
			free(imageAux);
			*error = CALLOC_ERROR;
			return NULL;
		}
		for (i = 0; i < k; i++) {
			mat[i] = calloc(sizeof(int), k + 1);
			if (mat[i] == NULL) {
				free(mat);
				free(originalImage);
				free(imageAux);
				*error = CALLOC_ERROR;
				return NULL;
			}
		}
		for (shadowImagesIndex = 0; shadowImagesIndex < k; shadowImagesIndex++) {
			image = getImage(shadowImages[shadowImagesIndex]);
			auxB = 0;
			shadowVector = calloc(sizeof(int), k);
			vectorIndex = 0;
			for (index = 0; index < k - 1; index++) {
				if (k == 3) {
					mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfAi) >> 3);
					shadowVector[vectorIndex++] = (int) image[sizeIndex + index];
				} else if (k == 2) {
					mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfAi) >> 4);
					shadowVector[vectorIndex++] = (int) image[sizeIndex + index];
				}
				auxB += ((int) image[sizeIndex + index]) & amountOfBi;
				
				if (k == 2) {
					auxB = auxB << 4;
				} else if (k == 3) {
					if (index == k - 2) {
						auxB = auxB << 2;
					} else {
						auxB = auxB << 3;
					}
				}
			}
			if (k == 3) {
				mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfLastAi) >> 3);
				shadowVector[vectorIndex++] = (int) image[sizeIndex + index];
			} else if (k == 2) {
				mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfLastAi) >> 5);
				shadowVector[vectorIndex++] = (int) image[sizeIndex + index];
			}
			auxB += ((int) image[sizeIndex + index]) & amountOfLastBi;
			mat[shadowImagesIndex][++index] = auxB % 251;
			checkPValue(shadowVector, k, error);

		}
		if (k == 3) {
			values = valuesFork3(mat, k, error);
		} else if (k == 2) {
			values = valuesFork2(mat, k, error);
		}
		
		for(i = 0; i < k; i++) {
			setImageInIndex(originalImage, (char) values[i], i + sizeIndex);
		}
	}
	if (globalP == FALSE) {
		printf("Cuidado que la imagen pudo haber sido alterada\n");
	}

	return originalImage;	
}

void checkPValue(int * values, int k, int * error) {
	int lastBytes, bBytes, i, p, newP, pValue;
	char * z, * valueForHast;
	unsigned char * md, * hash;
	if (EIGHT_BITS % k == 0) {
		lastBytes = EIGHT_BITS / k;
		bBytes = lastBytes;
	} else {
		lastBytes = EIGHT_BITS % k;
		bBytes = k;
	}
	p = pow(2, lastBytes);
	valueForHast = calloc(sizeof(char), (EIGHT_BITS * k) + 1);
	for (i = 0; i < k - 1; i++) {
		strcat(valueForHast, byte_to_binary(values[i]));
	}
	z = byte_to_binary(values[k-1]);
	strncat(valueForHast, z, EIGHT_BITS - lastBytes - 1);
	strcat(valueForHast, z + EIGHT_BITS - lastBytes);
	strcat(valueForHast, "0");

 	md = malloc(MD5_DIGEST_LENGTH); 
 	if (md == NULL) {
 		*error = MALLOC_ERROR;
 		return;
 	}
 	hash = MD5(valueForHast, strlen(valueForHast), md);

	newP = xorFromHashWrapper(hash, error, 8);
	if (*error != NO_ERROR) {
		return;
	}
	pValue = values[k-1] & p;

	if (pValue == 0) {
		if (newP != 0) {
			globalP = FALSE;;
		}
	} else if (newP == 0) {
		globalP = FALSE;
	}
}

int *
valuesFork2(int ** mat, int k, int * error) {
	int i, j, * values, deltaS, inverse, deltaX, deltaY, ** mat2;
	values = calloc(sizeof(int), k);
	if (values == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	deltaS = det2x2(mat);
	inverse = calculate_inverse(deltaS);
	mat2 = calloc(sizeof(int *), k);
	if (mat2 == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	for (i = 0; i < k; i++) {
		mat2[i] = calloc(sizeof(int), k + 1);
		if (mat2[i] == NULL) {
			free(mat);
			*error = CALLOC_ERROR;
			return NULL;
		}
	}
	for (i = 0; i < k; i++) {
		for (j = 0; j < k + 1; j++) {
			mat2[i][j] = mat[i][j];
		}
	} 
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat2[i][2];
	}
	deltaX = det2x2(mat2);
	values[0] = (deltaX * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat[i][0];
	}
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat2[i][2];
	}
	deltaY = det2x2(mat2);
	values[1] = (deltaY * inverse) % 251;
	return values;
}


int *
valuesFork3(int ** mat, int k, int * error) {
	int i, j, deltaS, deltaX, deltaY, deltaZ, inverse, ** mat2;
	int * values = calloc(sizeof(int), k);
	if (values == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	deltaS = detVertical3x3(mat);
	inverse = calculate_inverse(deltaS);
	mat2 = calloc(sizeof(int *), k);
	if (mat2 == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	for (i = 0; i < k; i++) {
		mat2[i] = calloc(sizeof(int), k + 1);
		if (mat2[i] == NULL) {
			free(mat);
			*error = CALLOC_ERROR;
			return NULL;
		}
	}
	for (i = 0; i < k; i++) {
		for (j = 0; j < k + 1; j++) {
			mat2[i][j] = mat[i][j];
		}
	} 
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat2[i][3];
	}
	deltaX = detHorizontal3x3(mat2);
	values[0] = (deltaX * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat[i][0];
	}
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat2[i][3];
	}
	deltaY = detVertical3x3(mat2);
	values[1] = (deltaY * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat[i][1];
	}
	for(i = 0; i < k; i++) {
		mat2[i][2] = mat2[i][3];
	}
	deltaZ = detHorizontal3x3(mat2);
	values[2] = (deltaZ * inverse) % 251;
	return values;
}


