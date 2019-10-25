CC     = gcc
CFLAGS = -Wall -ansi -O2 -pedantic -Wno-unused-result
RM     = rm
#------------------------------------------------

# chama o linker
pitao:  util.o objetos.o stack.o lexer.o eval.o main.o 
	$(CC) util.o objetos.o stack.o lexer.o eval.o main.o -o pitao -lm

main.o: main.c categorias.h util.h objetos.h lexer.h   
	$(CC) $(CFLAGS) -c main.c 

eval.o: eval.c eval.h util.h objetos.h categorias.h stack.h
	$(CC) $(CFLAGS) -c eval.c 

lexer.o: lexer.c categorias.h util.h objetos.h lexer.h 
	$(CC) $(CFLAGS) -c lexer.c 

objetos.o: objetos.c categorias.h util.h objetos.h
	$(CC) $(CFLAGS) -c objetos.c 

stack.o: stack.c util.h objetos.h stack.h 
	$(CC) $(CFLAGS) -c stack.c

util.o: util.c categorias.h util.h
	$(CC) $(CFLAGS) -c util.c 

clean:
	$(RM) *.o *~ pitao
