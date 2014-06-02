#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/constant.h"
void help();

// PARAMETRO 0: nombre del programa.
// PARAMETRO 1: -d, -r.
// PARAMETRO 2: -secret imagen.
// PARAMETRO 3: -k numero.
// PARAMETRO 4 (opcional): -n numero.
// PARAMETRO 5 (opcional): -dir directorio.

int
main(int argc, char * argv[]) {
	int method, k, n;
	char * filename = NULL;
	FILE * image = NULL;

	if(argc < 6 || argc > 10) {
		help();
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1],"-d") == 0) {
		method = ENCODE; // SE VA A CODIFICAR LA IMAGEN EN OTRAS
	} else if(strcmp(argv[1],"-r") == 0) {
		method = DECODE; // SE VA A DECODIFICAR UNA IMAGEN A PARTIR DE OTRAS
	} else {
		help();
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[2],"-secret") != 0) {
		help();
		exit(EXIT_FAILURE);
	}

	filename = argv[3];
	if(strstr(filename, ".bmp") == NULL) {
		printf("El archivo debe tener extensión .bmp\n");
		exit(EXIT_FAILURE);
	}
	if(method == ENCODE) {
		if((image = fopen(filename, "rb")) == NULL) {
			printf("No se encontró el archivo\n");
			exit(EXIT_FAILURE);
		}
	} else {
		image = fopen(filename, "wb");
	}
	

	if(strcmp(argv[4],"-k") != 0) {
		help();
		exit(EXIT_FAILURE);
	}

	k =  atoi(argv[5]);
	if(k != 2 && k != 3 && k != 4) {
		help();
		exit(EXIT_FAILURE);
	}
	
	if(argc > 6) {
		switch(argc) {
			case 8:
				if(strcmp(argv[6], "-n") == 0) {
					n = atoi(argv[7]);
					if(n < 3 || n > 8 || method == DECODE) {
						help();
						exit(EXIT_FAILURE);
					}
				} else if(strcmp(argv[6], "-dir") == 0) {
					// SE GUARDA EL DIRECTORIO DE DONDE SE LEEN O DONDE SE ESCRIBEN LAS IMAGENES CON EL SECRETO argv[7]
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
				if(n < 3 || n > 8 || method == DECODE) {
					help();
					exit(EXIT_FAILURE);
				}
				if(strcmp(argv[8], "-dir") != 0) {
					help();
					exit(EXIT_FAILURE);
				}
				break;
			default:
				help();
				exit(EXIT_FAILURE);
		}
	} else {
		// SE ELIGEN LOS PARAMETROS DEFAULT QUE DICE LA CONSIGNA
		// SERIAN: EL N EN CASO DE QUE SE HAYA USADO -D Y EL DIRECTORIO ACTUAL PARA LAS IMAGENES
	}
	
	
	// ACA SE CARGAN LAS IMAGENES DEL DIRECTORIO
	
	fclose(image);
	return EXIT_SUCCESS;	

	//TODO: Modularizar un poquito mas


	
}

void
help() {
	printf("Parámetros inválidos. Por favor recuerde la sintaxis del programa\nTP2 arg1 arg2 arg3 [arg4] [arg5] donde:\n\n");
	printf("arg1 = -d: indica que se va a distribuir una imagen secreta en otras imágenes\n");
	printf("%88s","-r: indica que se va a recuperar una imagen secreta a partir de otras imágenes\n\n");
	printf("arg2 = -secret imagen: el nombre imagen corresponde al nombre de un archivo de extensión .bmp. En el caso de que se haya elegido la opción (-d) éste archivo debe existir ya que es la imagen a ocultar. Si se eligió la opción (-r) éste archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa\n\n");
	printf("arg3 = -k número: el número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n). Sólo se tendrá en cuenta un valor de k igual a 2, 3 ó 4\n\n");
	printf("arg4 = -n número: el número corresponde a la cantidad total de sombras en las que se distribuirá el secreto en un esquema (k, n). Sólo puede usarse en el caso de que se haya elegido la opción (-d). Si no se usa, el programa elegirá como valor de n la cantidad total de imágenes del directorio. El valor de n será de, mínimo 3 y máximo 8\n\n");
	printf("arg5 = -dir directorio: el directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción (-d)), o donde están las imágenes que contienen oculto el secreto ( en el caso de que se haya elegido la opción (-r)). Si no se usa, el programa buscará las imágenes en el directorio actual\n\n");
}
