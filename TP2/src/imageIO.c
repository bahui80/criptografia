#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/constants.h"
#include "../inc/imageIO.h"

Image
loadImage(char * path, int * error) {
	int fileSize, offset;
	BYTE * header, * image;
	Image imageStruct = NULL;
	FILE * file = fopen(path, "rb");
	if (file == NULL) {
		*error = FILE_OPEN_ERROR;
		return NULL;
	}
	
	fseek(file, 2, SEEK_CUR); /* ME MUEVO 2 BYTES PARA LLEGAR AL TAMAÑO TOTAL DEL ARCHIVO */
	fread(&fileSize,  sizeof(int), 1, file);
	fseek(file, 4, SEEK_CUR); /* ME MUEVO 4 BYTES Y LLEGO AL VALOR DEL OFFSET */
	fread(&offset, sizeof(int), 1, file);
	
	rewind(file);
	header = calloc(offset + 1, sizeof(BYTE));
	if (header == NULL) {
		fclose(file);
		*error = CALLOC_ERROR;
		return NULL;
	}
	fread(header, sizeof(BYTE), offset, file);
	image = calloc((fileSize - offset) + 1, sizeof(BYTE));
	if (image == NULL) {
		fclose(file);
		free(header);
		*error = CALLOC_ERROR;
		return NULL;
	}
	fread(image, sizeof(BYTE), fileSize - offset, file);
	

	imageStruct = initialize(path, fileSize, offset, header, image, error);
	free(header);
	free(image);
	fclose(file);
	
	if (imageStruct == NULL) {
		return NULL;
	}
	
	return imageStruct;	
}
