#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "../inc/constants.h"

typedef struct image_t * IMAGE;
IMAGE initialize(char * filename, int filesize, int offset, BYTE * header, BYTE * image);
int getFilesize(IMAGE image);
int getOffset(IMAGE image);
BYTE * getHeader(IMAGE image);
BYTE * getImage(IMAGE image);
char * getFilename(IMAGE image);

#endif
