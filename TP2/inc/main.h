#ifndef __MAIN_H__
#define __MAIN_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "constants.h"
#include "imageIO.h"
#include "image.h"
#include "tests.h"
#include "provisorio.h"
#include "secretSharing2.h"


void help();
int readFilesFromDirectory(char * directory, int n, Image * shadowImages, int * error);
void printError(int error);
char * toLowerString(char * string, int * error);

#endif


