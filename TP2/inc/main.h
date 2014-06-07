#ifndef __MAIN_H__
#define __MAIN_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "../inc/constants.h"
#include "../inc/imageIO.h"
#include "../inc/image.h"
#include "../inc/tests.h"

void help();
int readFilesFromDirectory(char * directory, int n, Image * shadowImages, int * error);
void printError(int error);
char * toLowerString(char * string, int * error);

#endif


