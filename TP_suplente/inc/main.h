#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "bitmap.h"
#include "constants.h"
#include "share_secret.h"

void help();
int readFilesFromDirectory(char * directory, int n, simple_8bits_BMP_t ** shadowFiles, int * error);
void printError(int error);
char * toLowerString(char * string, int * error);

#endif
