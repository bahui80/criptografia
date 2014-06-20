#include "../inc/provisorio.h"
#include "../inc/secretSharing2.h"

void
saveImage(Image image, char * path, int * error);

int
main(int argc, char ** argv) {
	int k = 3, i;
	int error = NO_ERROR;
	int n = 3;
	char * directory = "../compare";
	Image * shadowImages = calloc(SHADOW_IMAGES, sizeof(Image));

	if (shadowImages == NULL) {
		error = CALLOC_ERROR;
		return EXIT_FAILURE;
	}
	// Image secretImage = loadImage("../Salma.bmp", &error);
	int amount = 0;
	int imagesRead = readFilesFromDirectory(directory == NULL ? "." : directory, n == 0 ? 8 : n, shadowImages, &error);
	for (i = 0; i < getFilesize(shadowImages[0]) - getOffset(shadowImages[0]); i++) {
		if (getImage(shadowImages[0])[i] != getImage(shadowImages[1])[i]) {
			// setImageInIndex(shadowImages[0], getImage(shadowImages[1])[i], i);
			printf("diff: %X - %X at %d\n", getImage(shadowImages[0])[i], getImage(shadowImages[1])[i], i);
			amount++;
		}
	}
	// saveImage(shadowImages[0], "./originalSalma2.bmp", &error);
	printf("Amount: %d\n", amount);
	return EXIT_SUCCESS;
	
}

int
readFilesFromDirectory(char * directory, int n, Image * shadowImages, int * error) {
	DIR * dir = NULL, * auxDir = NULL;
	int imagesRead = 0;
	char * fullPath = NULL;
	struct dirent * file = NULL;
	Image shadowImage = NULL;
	
	if((dir = opendir(directory)) != NULL) {
		while((file = readdir(dir)) != NULL && imagesRead < n) {
			fullPath = calloc(strlen(file->d_name) + strlen(directory) + 2, sizeof(char));
			if (fullPath == NULL) {
				*error = CALLOC_ERROR;
				return -1;
			}
			strncat(fullPath, directory, strlen(directory));
			if(directory[strlen(directory) - 1] != '/') {
				strncat(fullPath, "/", 1);			
			}
			strncat(fullPath, file->d_name, strlen(file->d_name));
			if((auxDir = opendir(fullPath)) == NULL) {
				/**
				 *	Con esto chequeo que lo que abri sea un archivo y no una carpeta
				 */
				char * d_name = toLowerString(file->d_name, error);
				if (*error != NO_ERROR) {
					return -1;
				}
				if (strstr(d_name, BMP)) {
					shadowImage = loadImage(fullPath, error);
					if (*error != NO_ERROR) {
						closedir(dir);
						free(fullPath);
						return -1;
					}
					shadowImages[imagesRead] = clone(shadowImage, error);
					if (*error != NO_ERROR) {
						closedir(dir);
						free(fullPath);
						free(shadowImage);
						return -1;
					}
					imagesRead++;
				}
			} else {
				closedir(auxDir);
			}
		}
		closedir(dir);
		free(fullPath);
	} else {
		*error = DIR_ERROR;
		return -1;
	}
	return imagesRead;
}

char *
toLowerString(char * string, int * error) {
	int i;	
	char * out = calloc(strlen(string) + 1, sizeof(char));
	if (out == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}

	for(i = 0; i < strlen(string); i++) {
		out[i] = tolower(string[i]);		
	}
	
	return out;
}

void
saveImage(Image image, char * path, int * error) { // EL PATH ES DONDE SE VA A GUARDAR EL ARCHIVO, ESO SE RECIBIO POR PARAMETRO
	int offset = getOffset(image);
	int fileSize = getFilesize(image);
	FILE * file;

	file = fopen(path, "wb");
	if(file == NULL) {
		*error = CALLOC_ERROR; // ACA PONER EL ERROR QUE CORRESPONDA
	}	

	fwrite(getHeader(image), sizeof(BYTE), offset, file);
	fwrite(getImage(image), sizeof(BYTE), fileSize - offset, file);

	fclose(file);
	*error = NO_ERROR;
}