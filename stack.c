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

  Nome:Gabriel Henrique Pinheiro Rodrigues

  stack.c
  Pitao I

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.

  Exemplo:
  - função mallocc retirada de: 
 
    http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* interface para o uso de uma pilha */
#include "stack.h" 



Stack stackInit() {
  /* Célula cabeça */
  Stack p = mallocSafe(sizeof(CelObjeto));
  return p;
}


int stackEmpty(Stack s) {
  if(s->prox == NULL)
    return 1;
  return 0;
}

void stackPush(Stack s, CelObjeto* celObj) {
  Stack aux = s->prox;
  s->prox = celObj;
  celObj->prox = aux;
}

CelObjeto* stackPop(Stack s) {
  CelObjeto* objRem;
  objRem = s->prox;
  s->prox = s->prox->prox;
  return objRem;
}

CelObjeto* stackTop(Stack s) {
  CelObjeto* objTopo = s->prox;
  return objTopo;
}

void stackFree(Stack s) {
  Stack aux = s->prox;
  while(aux != NULL){
    aux = aux->prox;
    printf("chegou no free do stack");
    free(s->prox);
    s->prox = aux;
  }
}

/*
void stackDump(Stack s) {
  Stack aux = s->prox;
  printf("Pilha: ");
  if(aux == NULL){
    printf("vazia.");
    return;
  }
  while(aux != NULL) {
    printf("");
  }
}
*/


/* 
 * 
 * STACK.c: IMPLEMENTACAO DE UMA PILHA 
 *
 * TAREFA
 *
 * Faca aqui a implementacao de uma pilha atraves de uma 
 * __lista encadeada com cabeca__. 
 * 
 * A implementacao deve respeitar a interface que voce declarar em 
 * stack.h. 
 *
 */

