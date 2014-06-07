#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/image.h"
#include "../inc/constants.h"

/**
* Test que crea las imagenes de nuevo en base a la estructura. Este test habria que incluirlo al final del main,
* cuando ya se creo el vector de estructuras shadowImages. Este test recorre ese vector de estructuras y crea en el
* directorio actual las imagenes que tiene la estructura para ver si se guardaron bien.
* Para correrlo se le pasa el shadowImages y en quantity va el imagesRead (que es la cantidad de imagenes que tiene el vector).
*/

void
checkImages(Image * shadowImages, int quantity) {
	int i;		
	FILE * file = NULL;
	char * filename;
	char * newFilename;
	const char * numbers[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
	
	for(i = 0; i < quantity; i++) {
		filename = getFilename(shadowImages[i]);	
		newFilename = calloc(strlen("image") + 2, sizeof(char));
		strcpy(newFilename, "image");
		strcat (newFilename, numbers[i]);
		file = fopen(newFilename,"wb");
		fwrite(getHeader(shadowImages[i]), sizeof(BYTE), getOffset(shadowImages[i]), file);
		fwrite(getImage(shadowImages[i]), sizeof(BYTE), getFilesize(shadowImages[i]) - getOffset(shadowImages[i]), file);
		fclose(file);
	}
}
