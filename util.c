/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 */

#include <stdlib.h> /* exit(), malloc(), EXIT_FAILURE */ 
#include <stdio.h>  /* fprinf(), stderr */
#include "util.h"   /* interface para este modulo */

/*-------------------------------------------------------------*/ 
/*  mallocSafe 

   O parâmetro de mallocSafe é do tipo size_t.  
   Em muitos computadores, size_t é equivalente a unsigned int.  
   A função mallocSafe não está em nenhuma biblioteca e é desconhecida 
   fora destas notas de aula. 
   Ela é apenas uma abreviatura conveniente.
   Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 */

void *
mallocSafe (size_t nbytes)
{
   void *ptr;

   ptr = malloc(nbytes);
   if (ptr == NULL) 
   {
       ERRO(Socorro! malloc devolveu NULL!);
       exit(EXIT_FAILURE);
   }

   return ptr;
}

