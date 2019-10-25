/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 * Declaracoes da funcao que substitui um item lexico pelo seu 
 * "valor" e da funcao que calcula o valor de uma expressao estao 
 * neste arquivo. A implementacao dessas funcoes esta' em 
 * eval.c
*/

#ifndef _EVAL_H
#define _EVAL_H

#include "objetos.h" /* CelObjeto */

/*-------------------------------------------------------------*/ 
/*  prototipos de funcoes e macros para funcoes                */

void 
itensParaValores (CelObjeto *iniFila);

CelObjeto *
eval(CelObjeto *iniPosfixa, Bool mostrePilhaExecucao);

#endif /* _EVAL_H */
