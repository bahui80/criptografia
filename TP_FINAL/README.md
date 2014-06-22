# BLAKLEY'S SECRET SHARING

- Distribuir una imagen secreta de extensión “.bmp” en otras imágenes también

de extensión “.bmp” que serán las sombras en un esquema (k, n) de secreto 

compartido.

- Recuperar una imagen secreta de extensión “.bmp” a partir de k imágenes, también 

de extensión “.bmp”

## Como usar

Posicionarse en la carpeta src y ejecutar <code>make create_secret_sharing</code> para generar el archivo SecretSharing dentro de la carpeta src

### Argumentos

<code>./SecretSharing arg1 arg2 arg3 [arg4] [arg5]</code>

arg1 = -d: indica que se va a distribuir una imagen secreta en otras imágenes.
       -r: indica que se va a recuperar una imagen secreta a partir de otras imágenes.

arg2 = -secret imagen: el nombre imagen corresponde al nombre de un archivo de extensión .bmp. En el caso de que se haya elegido la opción (-d) éste archivo debe existir ya que es la imagen a ocultar. Si se eligió la opción (-r) éste archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa.

arg3 = -k número: el número corresponde a la cantidad mínima de sombras necesarias para recuperar el secreto en un esquema (k, n). Sólo se tendrá en cuenta un valor de k igual a 2 o 3.

arg4 = -n número: el número corresponde a la cantidad total de sombras en las que se distribuirá el secreto en un esquema (k, n). Sólo puede usarse en el caso de que se haya elegido la opción (-d). Si no se usa, el programa elegirá como valor de n la cantidad total de imágenes del directorio. El valor de n será de, mínimo 3 y máximo 8.

arg5 = -dir directorio: el directorio donde se encuentran las imágenes en las que se distribuirá el secreto (en el caso de que se haya elegido la opción (-d)), o donde están las imágenes que contienen oculto el secreto ( en el caso de que se haya elegido la opción (-r)). Si no se usa, el programa buscará las imágenes en el directorio actual.

#### Ejemplos

##### Distribucion
<code>./SecretSharing -d -secret ../Albert.bmp -k 3 -n 8 -dir ../images</code>

#### Recuperación

<code>./SecretSharing -r -secret ../recover2.bmp -k 3 -dir ../shadows</code>

#### Imagenes de ejemplos

- dimen: 600x600 o 300x300
- k_number: k2 o k3

En la carpeta images/{dimen}/paraOcultar/{k_number}/ se encuentran las imagenes donde se quiere ocultar el secreto ubicado en images/{dimen}/secreto/{k_number}.

Luego en la carpeta shadows/{dimen}/{k_number} se pueden encontrar las shadows obtenidas para las imagenes mencionadas anteriormente.

Por ultimo en la carpeta recover/{dimen}/{k_number} se encuentra la imagen recuperada con las shadows obtenidas