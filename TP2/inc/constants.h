#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#ifndef PROGRAM_OPTION
enum {
	DISTRIBUTE = 0, /* Means -d command*/
	RECOVER = 1		/* Means -r command*/
};
#endif

typedef unsigned char BYTE;

#ifndef ERRORS
enum {
	NO_ERROR = 100,
	MALLOC_ERROR = 201,
	CALLOC_ERROR,
	INITIALIZE_IMAGE_STRUCT_ERROR,
	FILE_OPEN_ERROR,
	BMP_FORMAT_ERROR,
	HELP_ERROR,
	K_ERROR,
	DIR_ERROR,
	AMOUNT_IMAGE_ERROR
};
#endif

#ifndef BOOLEAN
#define TRUE 1
#define FALSE !TRUE
#endif

#ifndef PROGRAM_PARAMETERS
#define DISTRIBUTE_CMD "-d"
#define RECOVER_CMD "-r"
#define SECRET "-secret"
#define K_VALUE "-k"
#define N_VALUE "-n"
#define DIR_VALUE "-dir"
#endif

#ifndef IMAGE_EXTENSION
#define BMP ".bmp"
#endif

#define SHADOW_IMAGES 8

#endif
