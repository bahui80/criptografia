#include "../inc/main.h"

/**
 *	PARAMETRO 0: nombre del programa.
 *	PARAMETRO 1: -d, -r.
 *	PARAMETRO 2: -secret imagen.
 *	PARAMETRO 3: -k numero.
 *	PARAMETRO 4 (opcional): -n numero.
 *	PARAMETRO 5 (opcional): -dir directorio.
 */

int
main(int argc, char * argv[]) {
	int method, k, n = 0, nAux, imagesRead, i = 0, j;
	int error = NO_ERROR;
	char * filename = NULL, * filenameAux = NULL;
	char * directory = NULL;
	FILE * image = NULL;
	simple_8bits_BMP_t * secretImage = NULL;
	simple_8bits_BMP_t ** shadowImages = NULL;

	if(argc < 6 || argc > 10) {
		error = HELP_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], DISTRIBUTE_CMD) == 0) {
		method = DISTRIBUTE; 
	} else if(strcmp(argv[1], RECOVER_CMD) == 0) {
		method = RECOVER; 
	} else {
		error = HELP_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}

	if(strcmp(argv[2], SECRET) != 0) {
		error = HELP_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}
	
	filename = argv[3];
	filenameAux = toLowerString(filename, &error);
	if (error != NO_ERROR) {
		printError(error);
		return EXIT_FAILURE;
	}
	if(strstr(filenameAux, BMP) == NULL) {
		error = BMP_FORMAT_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}

	if(method == DISTRIBUTE) {
		if((image = fopen(filename, "rb")) == NULL) {
			printf("%s\n", filename);
			error = FILE_OPEN_ERROR;
			printError(error);
			return EXIT_FAILURE;
		}
		fclose(image);
	}

	if(strcmp(argv[4], K_VALUE) != 0) {
		error = HELP_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}

	k =  atoi(argv[5]);
	if(k != 2 && k != 3) {
		error = HELP_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}
	
	if(argc > 6) {
		switch(argc) {
			case 8:
				if(strcmp(argv[6], N_VALUE) == 0) {
					n = atoi(argv[7]);
					if(n < 3 || n > 8 || method == RECOVER) {
						error = HELP_ERROR;
					printError(error);
						return EXIT_FAILURE;
					}
				} else if(strcmp(argv[6], DIR_VALUE) == 0) {
					directory = argv[7];
				} else {
					error = HELP_ERROR;
					printError(error);
					return EXIT_FAILURE;
				}
				break;
			case 10:
				if(strcmp(argv[6], N_VALUE) != 0) {
					error = HELP_ERROR;
					printError(error);
					return EXIT_FAILURE;
				}
				n = atoi(argv[7]);
				if(n < 3 || n > 8 || method == RECOVER) {
					error = HELP_ERROR;
					printError(error);
					return EXIT_FAILURE;
				}
				if(strcmp(argv[8], DIR_VALUE) != 0) {
					error = HELP_ERROR;
					printError(error);
					return EXIT_FAILURE;
				}
				directory = argv[9];
				break;
			default:
				error = HELP_ERROR;
				printError(error);
				return EXIT_FAILURE;
		}
	}
	
	if(n != 0 && k > n) {
		error = K_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}

	nAux = n;
	shadowImages = calloc(SHADOW_IMAGES, sizeof(simple_8bits_BMP_t*));

	if (shadowImages == NULL) {
		error = CALLOC_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}
	imagesRead = readFilesFromDirectory(directory == NULL ? "." : directory, n == 0 ? 8 : n, shadowImages, &error);
	if (error != NO_ERROR) {
		printError(error);
		return EXIT_FAILURE;
	}
	if(nAux != 0 && imagesRead != nAux) {
		error = AMOUNT_IMAGE_ERROR;
		printError(error);
		return EXIT_FAILURE;
	}
	

	if(method == DISTRIBUTE) {
		secretImage = load_bmp_from_file(filename);
		if(secretImage == NULL) {
			printf("Hubo un error al alocar memoria\n");
			return EXIT_FAILURE;
		}
		share_secret(k, imagesRead, secretImage, shadowImages);
 		while(shadowImages[i] != NULL) {
			char outname[256];
			sprintf(outname,"shadow_%d.bmp",i);
			save_bmp_to_file(shadowImages[i], outname);
			++i;
	    	}
	} else {
		printf("LLEGO\n");
		secretImage = load_bmp_from_file(filename);			
		for(j=0; j < (secretImage->dib_header->height); j++) {
			secretImage->img[i] = calloc(1, secretImage->dib_header->width);
		}
		printf("PASO\n");
		get_secret(imagesRead, shadowImages, secretImage);
		save_bmp_to_file(secretImage, filename);
	}

	/*checkImages(shadowImages, imagesRead); Uncomment if we want to test*/
	
	return EXIT_SUCCESS;
}

void
printError(int error) {
	switch(error) {
		case MALLOC_ERROR:
		case CALLOC_ERROR:
			printf("No hay mas memoria disponible\n");
			break;
		case FILE_OPEN_ERROR:
			printf("El archivo no existe o no se ha encontrado\n");
			break;
		case BMP_FORMAT_ERROR:
			printf("La extension del archivo debe ser .bmp\n");
			break;
		case HELP_ERROR:
			help();
			break;
		case AMOUNT_IMAGE_ERROR:
			printf("No hay la cantidad de imágenes ingresada por parámetro en el directorio\n");
			break;
		case K_ERROR:
			printf("El valor de \'K\' no puede ser mayor que el valor de \'N\'\n");
			break;
		case DIR_ERROR:
			printf("El directorio ingresado no existe\n");
			break;
		default:
			printf("Error desconocido\n");
	}
}

int
readFilesFromDirectory(char * directory, int n, simple_8bits_BMP_t ** shadowImages, int * error) {
	DIR * dir = NULL, * auxDir = NULL;
	int imagesRead = 0;
	char * fullPath = NULL;
	struct dirent * file = NULL;
	simple_8bits_BMP_t * shadowImage = NULL;
	
	if((dir = opendir(directory)) != NULL) {
		while((file = readdir(dir)) != NULL && imagesRead < n) {
			fullPath = calloc(strlen(file->d_name) + strlen(directory) + 2, sizeof(char));
			if (fullPath == NULL) {
				*error = CALLOC_ERROR;
				return -1;
			}
			strncat(fullPath, directory, strlen(directory));
			if(directory[strlen(directory) - 1] != '/') {
				strncat(fullPath, "/", 1);			
			}
			strncat(fullPath, file->d_name, strlen(file->d_name));
			if((auxDir = opendir(fullPath)) == NULL) {
				/**
				 *	Con esto chequeo que lo que abri sea un archivo y no una carpeta
				 */
				char * d_name = toLowerString(file->d_name, error);
				if (*error != NO_ERROR) {
					return -1;
				}
				if (strstr(d_name, BMP)) {
					shadowImage = load_bmp_from_file(fullPath);
					if(shadowImage == NULL) {
						printf("Hubo un error al alocar memoria\n");
						return EXIT_SUCCESS;
					}
					shadowImages[imagesRead] = shadowImage;
					imagesRead++;
				}
			} else {
				closedir(auxDir);
			}
		}
		closedir(dir);
		free(fullPath);
	} else {
		*error = DIR_ERROR;
		return -1;
	}
	return imagesRead;
}

char *
toLowerString(char * string, int * error) {
	int i;	
	char * out = calloc(strlen(string) + 1, sizeof(char));
	if (out == NULL) {
		*error = CALLOC_ERROR;
		return NULL;
	}

	for(i = 0; i < strlen(string); i++) {
		out[i] = tolower(string[i]);		
	}
	
	return out;
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
