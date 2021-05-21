CURRENT_DIR = $(shell pwd)
SOURCE_DIR = $(CURRENT_DIR)/src
INCLUDE_DIR = $(CURRENT_DIR)/inc
OBJECT_DIR = $(CURRENT_DIR)/obj
IMG_DIR = $(CURRENT_DIR)/img
ENC_DIR = $(CURRENT_DIR)/enc
BIN_DIR = $(CURRENT_DIR)/bin
IMG_CONVERT_NAME = ImageConverter
AES_NAME = AES
ENC_FILE_NAME = enc

FILE_CMD = -f
OUTPUT_CMD = -o
MODE_CMD = --mode

ECB_MODE = ECB
CTR_MODE = CTR
Custom_MODE = CTM

CC = gcc
MKDIR = mkdir -p 

install: clean makedir install_ext enc_make
test: clean makedir enc_make

enc:
	$(MKDIR) test_enc
	$(BIN_DIR)/$(ENC_FILE_NAME) $(FILE_CMD) $(IMG_DIR)/linux.jpeg $(OUTPUT_CMD) $(CURRENT_DIR)/test_enc $(MODE_CMD) $(ECB_MODE)

#dec:
#	$(MKDIR) test_dec

makedir:
	$(MKDIR) obj
	$(MKDIR) bin
	$(MKDIR) tmp

install_ext:
	pip3 install Pillow
	pip3 install pycryptodome

enc_make: $(SOURCE_DIR)/$(ENC_FILE_NAME).c $(IMG_CONVERT_NAME).o $(AES_NAME).o
	$(CC) $(SOURCE_DIR)/$(ENC_FILE_NAME).c $(OBJECT_DIR)/$(IMG_CONVERT_NAME).o $(OBJECT_DIR)/$(AES_NAME).o -o $(BIN_DIR)/$(ENC_FILE_NAME) -I$(INCLUDE_DIR)  -pthread

$(IMG_CONVERT_NAME).o: $(SOURCE_DIR)/$(IMG_CONVERT_NAME).c
	$(CC) -c $(SOURCE_DIR)/$(IMG_CONVERT_NAME).c -o $(OBJECT_DIR)/$(IMG_CONVERT_NAME).o -I$(INCLUDE_DIR) 

$(AES_NAME).o: $(SOURCE_DIR)/$(AES_NAME).c
	$(CC) -c $(SOURCE_DIR)/$(AES_NAME).c -o $(OBJECT_DIR)/$(AES_NAME).o -I$(INCLUDE_DIR) -pthread

clean:
	rm -f $(BIN_DIR)/$(ENC_FILE_NAME)
	rm -f $(OBJECT_DIR)/$(IMG_CONVERT_NAME).o
	rm -f $(OBJECT_DIR)/$(AES_NAME).o
	
