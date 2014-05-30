#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void help();


// PARAMETRO 0: nombre del programa.
// PARAMETRO 1: -d, -r.
// PARAMETRO 2: -secret imagen.
// PARAMETRO 3: -k numero.
// PARAMETRO 4 (opcional): -n numero.
// PARAMETRO 5 (opcional): -dir directorio.

int
main(int argc, char * argv[]) {
	int i;

	if(argc < 6 || argc > 10) {
		help();
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1],"-r") == 0) {
		// LO GUARDO DE ALGUNA MANERA
	} else if(strcmp(argv[1],"-d") == 0) {
		// LO GUARDO DE ALGUNA MANERA
	} else {
		help();
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[2],"-secret") != 0) {
		help();
		exit(EXIT_FAILURE);
	}

	// argv[3] = "GUARDO EL NOMBRE DE ARCHIVO";

	if(strcmp(argv[4],"-k") != 0) {
		help();
		exit(EXIT_FAILURE);		
	}

	// argv[5] = "GUARDO EL NUMERO QUE REPRESENTA A k";


	return EXIT_SUCCESS;	
}

void
help() {
	printf("Parámetros inválidos. Por favor recuerde la sintaxis del programa\nTP2 arg1 arg2 arg3 [arg4] [arg5] donde:\n\n");
	printf("arg1 = -d: indica que se va a distribuir una imagen secreta en otras imágenes\n");
	printf("%88s","-r: indica que se va a recuperar una imagen secreta a partir de otras imágenes\n\n");
	printf("arg2 = -secret imagen: el nombre imagen corresponde al nombre de un archivo de extensión .bmp. En el caso de que se haya elegido la opción (-d) éste archivo debe existir ya que es la imagen a ocultar. Si se eligió la opción (-r) éste archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa\n\n");
	printf("arg3 = -k número: el número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n)\n\n");
	printf("arg4 = -n número: el número corresponde a la cantidad total de sombras en las que se distribuirá el secreto en un esquema (k, n). Sólo puede usarse en el caso de que se haya elegido la opción (-d). Si no se usa, el programa elegirá como valor de n la cantidad total de imágenes del directorio\n\n");
	printf("arg5 = -dir directorio: el directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción (-d)), o donde están las imágenes que contienen oculto el secreto ( en el caso de que se haya elegido la opción (-r)). Si no se usa, el programa buscará las imágenes en el directorio actual\n\n");
}