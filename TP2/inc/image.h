#ifndef __IMAGE_H__
#define __IMAGE_H__
#include "../inc/constants.h"

typedef struct image_t * Image;

Image initialize(char * filename, int filesize, int offset, BYTE * header, BYTE * image, int * error);
int getFilesize(Image image);
int getOffset(Image image);
BYTE * getHeader(Image image);
BYTE * getImage(Image image);
char * getFilename(Image image);

#endif
