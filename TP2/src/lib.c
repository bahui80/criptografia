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
    //  for (i = 0; i < 3; i++) {
    //      for (j = 0; j < 3 + 1; j++) {
    //          printf("%d - ", mat[i][j]);
    //      }
    //      printf("\n");
    //  }
    //  printf("Termina\n");
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
    //  for (i = 0; i < 3; i++) {
    //      for (j = 0; j < 3 + 1; j++) {
    //          printf("%d - ", mat[i][j]);
    //      }
    //      printf("\n");
    //  }
    //  printf("Termina\n");
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