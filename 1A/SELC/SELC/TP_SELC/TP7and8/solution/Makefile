## Definifion of variables

CC=gcc
LD=gcc
AR=ar
RM=rm

SRC_DIR=./src
LIB_DIR=./lib
INCLUDE_DIR=./include

CFLAGS= -Wall -c -I$(INCLUDE_DIR)
LDFLAGS=

LIB_FILES=liblinked-list-utils.a

EXEC_NAME=linked-list-exe

## Target liblinked-list-utils.a
## archive linked-list.o into library liblinked-list-utils.a
$(LIB_DIR)/liblinked-list-utils.a: linked-list.o
	$(AR) -r $@ $^

## Target main.o
## compile main.c to produce an object file (main.o)
main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $<

## Target linked-list.o
## compile linked-list.c to produce an object file (linked-list.o)
linked-list.o: $(SRC_DIR)/linked-list.c
	$(CC) $(CFLAGS) $<

## Target all
## link liblinked-list-utils.a and main.o to produce the executable (linked-list-exe)
all: linked-list-exe

linked-list-exe: depend $(LIB_DIR)/liblinked-list-utils.a main.o
	$(LD) $(LIB_DIR)/liblinked-list-utils.a main.o -o $(EXEC_NAME)

## Target clean
clean:
	$(RM) *.o $(LIB_DIR)/*.a $(EXEC_NAME)

depend:
	${CC} -MM $(SRC_DIR)/*.c -I$(INCLUDE_DIR) > depend

-include depend
