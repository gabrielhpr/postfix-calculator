/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 * Aqui esta' o prototipo da funcao mallocSafe e a definicao
 * algumas macros de uso geral.
*/

#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>  /* size_t */

/*-------------------------------------------------------------*/
#define ITENS        0
#define VALORES      1
#define PILHA        2


/*---------------------------------------------------------------*/
/* Bool */
enum bool 
{
    FALSE,
    TRUE
};
typedef enum bool Bool;

/*---------------------------------------------------------------*/
/* String */
typedef char * String;

/*-------------------------------------------------------------*/ 
/* alguma macros/abreviaturas uteis */
#define ERRO(msg)  fprintf(stderr,"ERRO: %s\n", #msg)
#define AVISO(msg) fprintf(stdout,"AVISO: %s\n", #msg)

/*---------------------------------------------------------------*/
/* prototipo de funcoes */

void 
*mallocSafe (size_t nbytes);

#endif /* _UTIL_H */

