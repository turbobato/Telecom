## Definifion of variables

CC=gcc -I$(INCLUDE_DIR)
LD=gcc -L$(LIB_DIR)
AR=ar
RM=/usr/bin/rm

SRC_DIR=./src
LIB_DIR=./lib
INCLUDE_DIR=./include

CFLAGS=-c -g -Wall
LDFLAGS=-g -llinked_list_utils

LIB_FILES=liblinked_list_utils.a

EXEC_NAME=linked-list-exe

## Target liblinked-list-utils.a
## archive linked-list.o into library liblinked-list-utils.a
$(LIB_DIR)/liblinked_list_utils.a:linked-lists.o
	$(AR) -r $(LIB_DIR)/$(LIB_FILES) linked-lists.o 

## Target main.o
## compile main.c to produce an object file (main.o)
main.o:$(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c

## Target linked-list.o
## compile linked-list.c to produce an object file (linked-list.o)
linked-lists.o:$(SRC_DIR)/linked-lists.c
	$(CC) $(CFLAGS) $(SRC_DIR)/linked-lists.c

## Target $(EXEC_NAME)
## link liblinked-list-utils.a and main.o to produce the executable (linked-list-exe)
$(EXEC_NAME):$(LIB_DIR)/liblinked_list_utils.a main.o
	$(LD) main.o $(LDFLAGS) -o $(EXEC_NAME)


## Target all: Useful? Where should be put in the maklib/liblinked-list-utils.aefile?
all: $(EXEC_NAME)


## Target clean
clean:
	$(RM) *.o $(LIB_DIR)/*.a $(EXEC_NAME)


depend: 
	$(CC) -MM $(SRC_DIR)/main.c -I $(INCLUDE_DIR) > depend
	$(CC) -MM $(SRC_DIR)/linked-lists.c -I $(INCLUDE_DIR) >> depend 

-include depend

