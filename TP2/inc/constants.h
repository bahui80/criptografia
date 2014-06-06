#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#ifndef PROGRAM_OPTION
enum {
	DISTRIBUTE = 0, // -d
	RECOVER = 1		// -r
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
	DIR_ERROR
}
#endif

#ifndef BOOLEAN
typedef TRUE 1
typedef FALSE !TRUE
#endif

#ifndef PROGRAM_PARAMETERS
typedef DISTRIBUTE_CMD = "-d";
typedef RECOVER_CMD = '-r';
typedef SECRET = "-secret";
typedef K_VALUE = "-k";
typedef N_VALUE = "-n";
typedef DIR_VALUE = "-dir";
#endif

#ifndef IMAGE_EXTENSION
typedef BMP = ".bmp";

#endif

#endif
