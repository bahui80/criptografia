#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/constants.h"
#include "../inc/imageIO.h"

IMAGE
loadImage(char * path) {
	FILE * file = fopen(path, "rb");
	int filesize, offset;
	BYTE * header, * image;
	
	fseek(file, 2, SEEK_CUR); // ME MUEVO 2 BYTES PARA LLEGAR AL TAMAÑO TOTAL DEL ARCHIVO
	fread(&filesize,  sizeof(int), 1, file);
	fseek(file, 4, SEEK_CUR); // ME MUEVO 4 BYTES Y LLEGO AL VALOR DEL OFFSET
	fread(&offset, sizeof(int), 1, file);
	
	rewind(file);
	header = calloc(offset + 1, sizeof(BYTE));
	fread(header, sizeof(BYTE), offset, file);
	image = calloc((filesize - offset) + 1, sizeof(BYTE));
	fread(image, sizeof(BYTE), filesize - offset, file);
	
	IMAGE imageStruct = initialize(path, filesize, offset, header, image);

	free(header);
	free(image);
	fclose(file);

	return imageStruct;	
}
