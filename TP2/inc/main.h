#ifndef __MAIN_H__
#define __MAIN_H__

void help();
int readFilesFromDirectory(char * directory, int n, IMAGE secretImage, IMAGE * shadowImages, int * error);
void printError(int error);

#endif


