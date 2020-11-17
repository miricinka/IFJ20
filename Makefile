  
PROJ=ifj20
CFLAGS=-std=gnu99 -Wall -Wextra -Werror
CC=gcc
LIBS=-lm
RM=rm -f
FILES=main.c scanner.c str.c error.c parser.c ilist.c prec_parser.c precedence_stack.c symtable.c

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES) $(LIBS)
	
clean:
	$(RM) *.o $(PROJ)