CFLAGS = -Wall -ansi -pedantic -O -Wuninitialized


SECRET_SHARING=../bin/SecretSharing

TARGET1=main
TARGET2=image
TARGET3=imageIO

CREATE_SECRET_SHARING=create_secret_sharing
REMAKE_SECRET_SHARING=remake_secret_sharing
CLEAN_SECRET_SHARING=clean_secret_sharing



help:
	@echo "Necesita ingresar alguno de los siguientes comandos"
	@echo "1) create_secret_sharing  ----- Compila y linkedita Secret sharing"
	@echo "2) remake_secret_sharing  ----- Borra, Compila y linkedita Secret sharing"
	@echo "3) clean_secret_sharing   ----- Borra el ejecutable de sercret sharing ubicado en ../bin"

$(CREATE_SECRET_SHARING):	$(TARGET1).c $(TARGET2).c $(TARGET3).c
	@gcc $(CFLAGS) -o $(SECRET_SHARING) $(TARGET1).c $(TARGET2).c $(TARGET3).c
	@echo "------------Secret sharing programm created in ../bin/SecretSharing----------------"

$(CLEAN_SECRET_SHARING):
	@rm -f $(SECRET_SHARING)
	@echo "-----------Secret sharing was remove------------"

$(REMAKE_SECRET_SHARING): $(CLEAN_SECRET_SHARING) $(CREATE_SECRET_SHARING)

