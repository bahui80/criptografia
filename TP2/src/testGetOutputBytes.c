#include "../inc/secretSharing.h"

int
main(int argc, char ** argv) {
	int k = 3;
	BYTE selectedSecretBytes[] = {110, 24, 72};
	BYTE selectedOutputBytes[] = {57, 99, 220};
	int i;

	int error = getOutputBytes(selectedSecretBytes, selectedOutputBytes, k);

	for (i = 0; i < k; i++) {
		printf("%d ", (int) selectedOutputBytes[i]);
	}
	printf("\n");

	return EXIT_SUCCESS;
}