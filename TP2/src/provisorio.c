#include "../inc/provisorio.h"

Image
recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error) {
	int superValue = 120;
	int lastBytes, bBytes, i, j;
	int amountOfLastBi = 0, amountOfBi = 0, p = 0, a = 0;
	int amountOfAi = 255, amountOfLastAi = 255;
	int * andB, auxAmount, auxAmountBytes, b, ai, shadowImagesIndex, index, sizeIndex;


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
	Image originalImage = clone(shadowImages[0], error);
	if (*error != NO_ERROR) {
		return NULL;
	}
	BYTE * imageAux = calloc(sizeof(BYTE), amountOfBytes);
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
			BYTE * image = getImage(shadowImages[shadowImagesIndex]);
			int auxB = 0;
			for (index = 0; index < k - 1; index++) {
				// if (sizeIndex == 2) {
				// 	printf("Image %d value = %X\n", shadowImagesIndex, image[sizeIndex + index]);
				// }
				if (k == 3) {
					mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfAi) >> 3);
				} else if (k == 2) {
					// if (sizeIndex == 2) {
					// 	printf("((((int) image[sizeIndex + index]) & amountOfAi) >> 4): %d\n", ((((int) image[sizeIndex + index]) & amountOfAi) >> 4));
					// 	printf("and: %s\n", byte_to_binary((((int) image[sizeIndex + index]) & amountOfAi)));
					// 	printf("amount: %s\n", byte_to_binary(amountOfAi));
					// 	printf("binary: %s\n", byte_to_binary((int) image[sizeIndex + index]));
					// }
					mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfAi) >> 4);
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
			// if (sizeIndex == 2) {
			// 	printf("Image %d value = %X\n", shadowImagesIndex, image[sizeIndex + index]);
			// 	printf("((((int) image[sizeIndex + index]) & amountOfLastAi) >> 5): %d\n", ((((int) image[sizeIndex + index]) & amountOfLastAi) >> 5));
			// 	printf("and: %s\n", byte_to_binary((((int) image[sizeIndex + index]) & amountOfLastAi)));
			// 	printf("amount: %s\n", byte_to_binary(amountOfLastAi));
			// 	printf("binary: %s\n", byte_to_binary((int) image[sizeIndex + index]));
			// }
			if (k == 3) {
				mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfLastAi) >> 3);
			} else if (k == 2) {
				mat[shadowImagesIndex][index] = ((((int) image[sizeIndex + index]) & amountOfLastAi) >> 5);
			}
			auxB += ((int) image[sizeIndex + index]) & amountOfLastBi;
			mat[shadowImagesIndex][++index] = auxB % 251;

		}
		// if (sizeIndex < 20) {
		// 	printf("Matri<\n");
		// 	for (i = 0; i < k; i++) {
		// 		for (j = 0; j < k + 1; j++) {
		// 			printf("%d - ", mat[i][j]);
		// 		}
		// 		printf("\n");
		// 	}
		// }
		int * values;
		if (k == 3) {
			values = valuesFork3(mat, k, error);
		} else if (k == 2) {
			values = valuesFork2(mat, k, error);
		}
		// if (sizeIndex < 20) {
		// 		printf("Values: x = %d y = %d deltaS = %d deltaX = %d  deltaY = %d\n", values[0], values[1], values[2], values[3], values[4]);
		// }
		for(i = 0; i < k; i++) {
			// printf("i + sizeIndex: %d values = %d\n", i + sizeIndex, values[i]);
			setImageInIndex(originalImage, (char) values[i], i + sizeIndex);
		}
	}

	return originalImage;	
}

int *
valuesFork2(int ** mat, int k, int * error) {
	int i, j;
	int * values = calloc(sizeof(int), k);
	if (values == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	int deltaS = det2x2(mat);
	int inverse = calculate_inverse(deltaS);
	int ** mat2 = calloc(sizeof(int *), k);
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
	int deltaX = det2x2(mat2);
	values[0] = (deltaX * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat[i][0];
	}
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat2[i][2];
	}
	int deltaY = det2x2(mat2);
	values[1] = (deltaY * inverse) % 251;
	values[2] = deltaS;
	values[3] = deltaX;
	values[4] = deltaY;
	return values;
}

int 
det2x2(int ** mat) {
	int left = (mat[0][0] * mat[1][1]) % 251;
	int right = (mat[0][1] * mat[1][0]) % 251;
	int result = (left - right);
	while (result < 0) {
		result += 251;
	}
	result = result % 251;
	return result;
}

int *
valuesFork3(int ** mat, int k, int * error) {
	int i, j;
	int * values = calloc(sizeof(int), k);
	if (values == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	int deltaS = detVertical3x3(mat);
	int inverse = calculate_inverse(deltaS);
	int ** mat2 = calloc(sizeof(int *), k);
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
	int deltaX = detHorizontal3x3(mat2);
	values[0] = (deltaX * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][0] = mat[i][0];
	}
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat2[i][3];
	}
	int deltaY = detVertical3x3(mat2);
	values[1] = (deltaY * inverse) % 251;
	for(i = 0; i < k; i++) {
		mat2[i][1] = mat[i][1];
	}
	for(i = 0; i < k; i++) {
		mat2[i][2] = mat2[i][3];
	}
	int deltaZ = detHorizontal3x3(mat2);
	values[2] = (deltaZ * inverse) % 251;
	return values;
}

// Vertical (3x3):
int
detVertical3x3(int ** mat) {
	// int mat[3][3] = {{3, 2, 1}, {0, 2, -5}, {-2, 1, 4}};
	int first_left = 1;
	int second_left = 1;
	int third_left = 1;
	int first_right = 1;
	int second_right = 1;
	int third_right = 1;
	int i;

	for (i = 0; i < 3; i++) {
		first_left *= mat[i % 3][i];
		second_left *= mat[(i + 1) % 3][i];
		third_left *= mat[(i + 2) % 3][i];


		first_right *= mat[2 - i][i % 3];
		second_right *= mat[2 - i][(i + 1) % 3];
		third_right *= mat[2 - i][(i + 2) % 3];
	}

	unsigned int left = (first_left + second_left + third_left) % 251;
	unsigned int right = (first_right + second_right + third_right) % 251;
	int result = (left - right);
	while (result < 0) {
		result += 251;
		// result = -result;
	}
	result = result % 251;
	// int j;
	// if (result == 0) {
	// 	for (i = 0; i < 3; i++) {
	// 		for (j = 0; j < 3 + 1; j++) {
	// 			printf("%d - ", mat[i][j]);
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("Termina\n");
	// }

	// printf("result: %d\n", result);
	return result;
}
// Horizontal (3x3):


int
detHorizontal3x3(int ** mat) {
	int first_left = 1;
	int second_left = 1;
	int third_left = 1;
	int first_right = 1;
	int second_right = 1;
	int third_right = 1;
	int i;

	for (i = 0; i < 3; i++) {
		first_left *= mat[i][i];
		second_left *= mat[i][(i + 1) % 3];
		third_left *= mat[i][(i + 2) % 3];

		first_right *= mat[i][2 - i];
		second_right *= mat[i][(i * 2) % 3];
		third_right *= mat[i][i == 2 ? 2 : (i + 1) % 2];
	}

	unsigned int left = (first_left + second_left + third_left) % 251;
	unsigned int right = (first_right + second_right + third_right) % 251;
	int result = (left - right);
	while (result < 0) {
		// printf("Negativo: %d\n", result);
		result += 251;
		// result = -result;
	}
	result = result % 251;
	// int j;
	// if (result == 0) {
	// 	for (i = 0; i < 3; i++) {
	// 		for (j = 0; j < 3 + 1; j++) {
	// 			printf("%d - ", mat[i][j]);
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("Termina\n");
	// }
	// printf("result: %d\n", result);
	return result;
}

int
calculate_inverse(int b)
{
	if(b == 0 || b == 251)
		return 0;
	int i = 1;
	int rta = 0;
	while((rta = (b*i) %251) != 1)
	{
		++i;
	}
	return i;
}


