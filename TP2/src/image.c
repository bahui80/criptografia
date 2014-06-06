#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/image.h"
#include "../inc/constants.h"

typedef struct {
	char * filename;
	int fileSize;
	int offset;
	BYTE * header;
	BYTE * image;
} image_t;

Image
initialize(char * filename, int fileSize, int offset, BYTE * header, BYTE * image, int * error) {
	Image imageStruct = malloc(sizeof(struct image_t));
	if (imageStruct == NULL) {
		*error = MALLOC_ERROR;
		return NULL;
	}
	
	imageStruct->filename = calloc(strlen(filename) + 1, sizeof(char));
	if (imageStruct->filename == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	strcpy(imageStruct->filename, filename);
	
	imageStruct->fileSize = fileSize;
	imageStruct->offset = offset;
	
	imageStruct->header = calloc(offset + 1, sizeof(BYTE));
	if (imageStruct->header == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	memcpy(imageStruct->header, header, offset);
	
	imageStruct->image = calloc(fileSize - offset + 1, sizeof(BYTE));
	if (imageStruct->image == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}
	memcpy(imageStruct->image, image, fileSize - offset);

	return imageStruct;
}

void
freeImage(Image image) {
	if (image != NULL) {
		if (image->header != NULL) {
			free(image->header);
		}
		if (image->filename != NULL) {
			free(image->filename);
		}
		if (image->image != NULL) {
			free(image->image);
		}
		free(image);
	}
}

int
getFilesize(Image image) {
	return image->fileSize;
}

int
getOffset(Image image) {
	return image->offset;
}

BYTE *
getHeader(Image image) {
	return image->header;
}

BYTE *
getImage(Image image) {
	return image->image;
}

char *
getFilename(Image image) {
	return image->filename;
}
