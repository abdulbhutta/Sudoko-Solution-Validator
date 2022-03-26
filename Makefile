CC = gcc
CFLAGS = -Wall -pthread -Wextra -std=c99
LFLAGS =
LIBS = 
SOURCES = Sudoko.c 
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = 
.PHONY: clean help

sudoko : sudoko.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"


