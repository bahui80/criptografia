Image
recoverSecretImage(Image * shadowImages, int amountOfBytes, int k, int * error) {
	int lastBytes, bBytes, i, *error = NO_ERROR, j;
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
	for (j = 0; j < lastBytes; j++) {
		amountOfLastBi += pow(2, j);
	}
	for (j = 0; j < bBytes; j++) {
		amountOfBi += pow(2, j);
	}
	amountOfAi -= amountOfBi;
	p = pow(2, lastBytes);
	amountOfLastAi -= (p + amountOfLastBi);


	// PSEUDOCODIGO
	for(l = 0; l < tamaño imagen; l += k) { // 
		// TODO: VALIDAR QUE EL BIT DE PARIDAD ESTE BIEN ANTES DE SEGUIR
		b1 = concat(amountOfBi,amountOfBi, amountOfLastBi); // SACO EL VALOR DE B
	
		a1 = amountOfAi; //SACO a1
		a2 = amountOfAi; // SACO a2 
		a3 = amountOfLastAi; // SACO a3

		a11 * x + a12 * y + a13 * z = b1
		a21 * x + a22 * y + a23 * z = b2
		a31 * x + a32 * y + a33 * z = b3
	
		solve([a] , [b], k);
	}	
}


Vertical (3x3):

int first_left = 1;
int second_left = 1;
int third_left = 1;
int first_right = 1;
int second_right = 1;
int third_right = 1;

for (i = 0; i < 3; i++) {
	first_left *= mat[i % 3][i];
	second_left *= mat[(i + 1) % 3][i];
	third_left *= mat[(i + 2) % 3][i];

	first_right *= mat[i][i % 3];
	second_right *= mat[i][(i + 1) % 3];
	third_right *= mat[i][(i + 2) % 3];
}

int left = (first_left + second_left + third_left);
int right = (first_right + second_right + third_right);
int result = mod((left - right), 251);


Horizontal (3x3):


int first_left = 1;
int second_left = 1;
int third_left = 1;
int first_right = 1;
int second_right = 1;
int third_right = 1;

for (i = 0; i < 3; i++) {
	first_left *= mat[i % 3][i];
	second_left *= mat[i % 3][i % 3];
	third_left *= mat[i % 3][(i + 2) % 3];

	first_right *= mat[i][2 - i];
	second_right *= mat[i][(i * 2) % 3];
	third_right *= mat[i][i == 2 ? 2 : (i + 1) % 2];
}

int left = (first_left + second_left + third_left);
int right = (first_right + second_right + third_right);
int result = mod((left - right), 251);

