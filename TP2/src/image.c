#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/image.h"
#include "../inc/constants.h"

struct image_t {
	char * filename;
	int filesize;
	int offset;
	BYTE * header;
	BYTE * image;
};

IMAGE
initialize(char * filename, int filesize, int offset, BYTE * header, BYTE * image) {
	IMAGE imageStruct = malloc(sizeof(struct image_t));
	
	imageStruct->filename = calloc(strlen(filename) + 1, sizeof(char));
	strcpy(imageStruct->filename, filename);
	
	imageStruct->filesize = filesize;
	imageStruct->offset = offset;
	
	imageStruct->header = calloc(offset + 1, sizeof(BYTE));
	memcpy(imageStruct->header, header, offset);
	
	imageStruct->image = calloc(filesize - offset + 1, sizeof(BYTE));
	memcpy(imageStruct->image, image, filesize - offset);

	return imageStruct;
}

int
getFilesize(IMAGE image) {
	return image->filesize;
}

int
getOffset(IMAGE image) {
	return image->offset;
}

BYTE *
getHeader(IMAGE image) {
	return image->header;
}

BYTE *
getImage(IMAGE image) {
	return image->image;
}

char *
getFilename(IMAGE image) {
	return image->filename;
}
