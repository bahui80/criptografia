#include <stdio.h>
#include <stdlib.h>


int 
main(void) {
	FILE * file = fopen("../images/Albert.bmp", "rb");
	char * hola = calloc(100000,sizeof(char));
	int fileSize, offset;	
	
	fseek(file,2, SEEK_CUR); // ME MUEVO 2 BYTES PARA LLEGAR AL TAMAÑO TOTAL DEL ARCHIVO
	
	fread(&fileSize, 4, 1, file);
	fseek(file,4, SEEK_CUR); // ME MUEVO 4 BYTES Y LLEGO AL OFFSET DONDE EMPIEZA LA IMAGEN EN SI SIN HEADERS NI BASURA
	fread(&offset, 4, 1, file);	
	printf("File size = %d\n",fileSize);
	printf("Offset = %d\n",offset);
	
//	while(!feof(file)) {
//		fread(hola, sizeof(char), 1, file);
//	}
	return 0;
}
