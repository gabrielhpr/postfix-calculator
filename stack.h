/*
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Gabriel Henrique Pinheiro Rodrigues

  stack.h
  Pitao I
cla
  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.

  Exemplo:
  - função mallocc retirada de: 

    http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * stack.h 
 * INTERFACE: funcoes para manipular uma pilha 
 */

#ifndef _STACK_H
#define _STACK_H

#include "objetos.h" /* CelObjeto */
#include "util.h" /*mallocSafe()*/
#include <stdlib.h> /*free()*/

typedef CelObjeto* Stack;

Stack stackInit();
int stackEmpty(Stack);
void stackPush(Stack, CelObjeto*);
CelObjeto* stackPop(Stack);
CelObjeto* stackTop(Stack);
void stackFree(Stack);
/*void stackDump(Stack);*/

/*
 * TAREFA
 *
 * Aqui voce deve colocar a interface para acesso a sua   
 * implementacao de uma pilha (stack.c).
 * 
 * Se julgar conveniente, defina o arquivo item.h o tipo Item
 * como nas notas de aula.
 */

#endif /* _STACK_H */
