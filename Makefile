PROJ=ifj20
CFLAGS=-std=gnu99 -Wall -Wextra -g
CC=gcc
LIBS=-lm
RM=rm -f
FILES=symtable_test.c symtable.c str.c

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES) $(LIBS)
	
clean:
	$(RM) *.o $(PROJ)
