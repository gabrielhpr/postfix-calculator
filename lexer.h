/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 *
 *  As declaracao da funcao responsavel pela analise lexica e
 *  a abreviatura 'item' estao neste arquivo.
 */

#ifndef _LEXER_H
#define _LEXER_H

#include "objetos.h"    /* CelObjeto */

/*------------------------------------------------------------*/
/* abreviatura para nos referenciarmos a um item lexico       */
#define item valor.pStr

/*-------------------------------------------------------------*/ 
/*  prototipos de funcoes                                      */

CelObjeto *
crieFilaItens (char linha[]);

#endif /* _LEXER_H */ 
