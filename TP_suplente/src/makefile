CFLAGS = -lm -lcrypto -lssl

SECRET_SHARING=../bin/SecretSharing

TARGET1=main
TARGET2=bitmap
TARGET3=bit_wise
TARGET4=memory_utils
TARGET5?=mod251_operations
TARGET6=share_secret
TARGET7=share_secret_utils

CREATE_SECRET_SHARING=create_secret_sharing
REMAKE_SECRET_SHARING=remake_secret_sharing
CLEAN_SECRET_SHARING=clean_secret_sharing



help:
	@echo "Necesita ingresar alguno de los siguientes comandos"
	@echo "1) create_secret_sharing  ----- Compila y linkedita Secret sharing"
	@echo "2) remake_secret_sharing  ----- Borra, Compila y linkedita Secret sharing"
	@echo "3) clean_secret_sharing   ----- Borra el ejecutable de sercret sharing ubicado en ../bin"

$(CREATE_SECRET_SHARING):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c
	@gcc -o $(SECRET_SHARING) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c $(CFLAGS) 
	@echo "------------Secret sharing programm created in ../bin/SecretSharing----------------"

$(CLEAN_SECRET_SHARING):
	@rm -f $(SECRET_SHARING)
	@echo "-----------Secret sharing was remove------------"

$(REMAKE_SECRET_SHARING): $(CLEAN_SECRET_SHARING) $(CREATE_SECRET_SHARING)

