#ifndef __MAIN_H__
#define __MAIN_H__

#include "../inc/image.h"

void help();
int readFilesFromDirectory(char * directory, int n, Image secretImage, Image * shadowImages, int * error);
void printError(int error);
char * toLowerString(char * string);

#endif


