#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../inc/constants.h"
void help();

// PARAMETRO 0: nombre del programa.
// PARAMETRO 1: -d, -r.
// PARAMETRO 2: -secret imagen.
// PARAMETRO 3: -k numero.
// PARAMETRO 4 (opcional): -n numero.
// PARAMETRO 5 (opcional): -dir directorio.

int
main(int argc, char * argv[]) {
	int method, k, n = 0;
	char * filename = NULL;
	FILE * image = NULL;
	char * directory = NULL;

	if(argc < 6 || argc > 10) {
		help();
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1],"-d") == 0) {
		method = DISTRIBUTE; 
	} else if(strcmp(argv[1],"-r") == 0) {
		method = RECOVER; 
	} else {
		help();
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[2],"-secret") != 0) {
		help();
		exit(EXIT_FAILURE);
	}

	filename = argv[3];
	if(strstr(filename, ".bmp") == NULL && strstr(filename, ".BMP") == NULL) {
		printf("El archivo debe tener extensión .bmp\n");
		exit(EXIT_FAILURE);
	}
	if(method == DISTRIBUTE) {
		if((image = fopen(filename, "rb")) == NULL) {
			printf("No se encontró el archivo\n");
			exit(EXIT_FAILURE);
		}
	}

	if(strcmp(argv[4],"-k") != 0) {
		help();
		exit(EXIT_FAILURE);
	}

	k =  atoi(argv[5]);
	if(k != 2 && k != 3) {
		help();
		exit(EXIT_FAILURE);
	}
	
	if(argc > 6) {
		switch(argc) {
			case 8:
				if(strcmp(argv[6], "-n") == 0) {
					n = atoi(argv[7]);
					if(n < 3 || n > 8 || method == RECOVER) {
						help();
						exit(EXIT_FAILURE);
					}
				} else if(strcmp(argv[6], "-dir") == 0) {
					directory = argv[7];
				} else {
					help();
					exit(EXIT_FAILURE);
				}
				break;
			case 10:
				if(strcmp(argv[6], "-n") != 0) {
					help();
					exit(EXIT_FAILURE);
				}
				n = atoi(argv[7]);
				if(n < 3 || n > 8 || method == RECOVER) {
					help();
					exit(EXIT_FAILURE);
				}
				if(strcmp(argv[8], "-dir") != 0) {
					help();
					exit(EXIT_FAILURE);
				}
				directory = argv[9];
				break;
			default:
				help();
				exit(EXIT_FAILURE);
		}
	}
	

	
	// TODO: PONER VALOR DEFAULT SERIAN: EL N EN CASO DE QUE SE HAYA USADO -D 
	
	
	readFilesFromDirectory(directory == NULL ? "." : directory, n);
	// TODO: SI ESTO PASO BIEN RECIEN ACA SE CREA LA IMAGEN EN CASO QUE HAYA QUE CREARLA
	
	fclose(image);
	return EXIT_SUCCESS;	

	//TODO: Modularizar un poquito mas
}

int
readFilesFromDirectory(char * directory, int n) {
	DIR * dir = NULL, * auxDir = NULL;
	int imagesRead = 0;
	char * fullPath = NULL;
	struct dirent * file = NULL;
	
	if((dir = opendir(directory)) != NULL) {
		while((file = readdir(dir)) != NULL && imagesRead < n) {
			fullPath = calloc(strlen(file -> d_name) + strlen(directory) + 2, sizeof(char));
			strncat(fullPath, directory, strlen(directory));
			if(directory[strlen(directory) - 1] != '/') {
				strncat(fullPath, "/", 1);			
			}
			strncat(fullPath, file -> d_name, strlen(file -> d_name));
			if((auxDir = opendir(fullPath)) == NULL) {
				// Con esto chequeo que lo que abri sea un archivo y no una carpeta
				if (strstr(file->d_name, ".bmp") != NULL || strstr(file->d_name, ".BMP") != NULL) {
					// TODO: CARGO LA IMAGEN ACA
					imagesRead++;
				}
			} else {
				closedir(auxDir);
			}
		}
	} else {
		printf("El directorio ingresado no existe\n");
		return -1;
	}
	closedir(dir);
	free(fullPath);
}

void
help() {
	printf("Parámetros inválidos. Por favor recuerde la sintaxis del programa\nTP2 arg1 arg2 arg3 [arg4] [arg5] donde:\n\n");
	printf("arg1 = -d: indica que se va a distribuir una imagen secreta en otras imágenes\n");
	printf("%88s","-r: indica que se va a recuperar una imagen secreta a partir de otras imágenes\n\n");
	printf("arg2 = -secret imagen: el nombre imagen corresponde al nombre de un archivo de extensión .bmp. En el caso de que se haya elegido la opción (-d) éste archivo debe existir ya que es la imagen a ocultar. Si se eligió la opción (-r) éste archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa\n\n");
	printf("arg3 = -k número: el número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n). Sólo se tendrá en cuenta un valor de k igual a 2 o 3\n\n");
	printf("arg4 = -n número: el número corresponde a la cantidad total de sombras en las que se distribuirá el secreto en un esquema (k, n). Sólo puede usarse en el caso de que se haya elegido la opción (-d). Si no se usa, el programa elegirá como valor de n la cantidad total de imágenes del directorio. El valor de n será de, mínimo 3 y máximo 8\n\n");
	printf("arg5 = -dir directorio: el directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción (-d)), o donde están las imágenes que contienen oculto el secreto ( en el caso de que se haya elegido la opción (-r)). Si no se usa, el programa buscará las imágenes en el directorio actual\n\n");
}
