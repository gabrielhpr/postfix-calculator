/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 *
 * Aqui esta' declarado o tipo CelObjeto.
 *
 * As celulas de __todas__ as listas (pilhas ou filas) deste EP
 * serao do tipo CelObjeto. 
 *
 * Aqui tambem estao os prototipos de funcoes que alocam, liberam,
 * e imprimem CelObjetos e que sao usadas em __todos__ os modulos.
 *
 */

#ifndef _OBJETOS_H
#define _OBJETOS_H

#include "categorias.h" /* Categoria */
#include "util.h"       /* Bool, String */

/*---------------------------------------------------------------*/
/* Um CelObjeto contem um valor que pode ser: 
 *
 *    - um inteiro ('vInt'   = valor int) ou
 *    - um float   ('vFloat' = valor double) ou
 *    - um string  ('pStr'   = valor string = ponteiro para char)
 */
union valor
{
    /* para tipos INT, BOOL (ignorar INT e BOOL) e precedencia de operadores (OPER_*) */
    int    vInt;    
    
    /* para tipo FLOAT: float em Python == double do C */ 
    double  vFloat;  
    
    /* para categorias que utilizam strings INT_STR, FLOAT_STR, 
       BOOL_STR e STR (ignorar STR) */
    String  pStr;    
};
typedef union valor Valor;


/*--------------------------------------------------------------*/
/* CelObjeto e' o tipo da celula utilizada para representar
 * todas as listas (pilhas ou filas) neste EP.
 *
 * Campo CATEGORIA
 *
 *     Indica a categoria a qual o objeto pertence. Pode ser
 *     um operador (OPER_*, um inteiro (INT), um double (FLOAT)... 
 *     Ver a lista de categorias em categorias.h.
 * 
 * Campo VALOR
 *
 *     Logo apos a analise lexica, o campo valor contem um 
 *     string (valor.pStr) que representa o item lexico.
 *
 *     Depois esses strings serao substituidos pela funcao 
 *     itensParaValores (eval.c) por outros valores.
 *     Esses outros valores dependem da categoria a que 
 *     o item pertence da seguinte maneira:
 *
 *          - INT   : valor(.vInt) contera' um inteiro;
 * (IGNORAR)- BOOL  : valor(.vInt) contera' um inteiro;
 *          - FLOAT : valor(.vFloat) contera' um double;
 * (IGNORAR)- STR   : valor(.pStr) contera' um string;
 *          - OPER_*: valor(.vInt) contera' um inteiro indicando
 *                    a precedencia do operador; 
 *  (EP4)   - ID    : valor(.pStr) contera' um ponteiro para um string
 *                    com o nome do identificador.
 * 
 * Campo PROX
 *
 *     Ponteiro para a proxima celula. 
 *
 */
typedef struct celObjeto CelObjeto;
struct celObjeto 
{
    Categoria  categoria;  /* indica a categoria do objeto */
    Valor          valor;  /* representacao do 'valor' do Objeto */
    CelObjeto      *prox;  /* ponteiro para a proxima celula */	
};

 
/*---------------------------------------------------------------*/
/* INTERFACE para o uso do modulo objetos.c                      */

void  
freeObjeto(CelObjeto *pObj);

void  
freeListaObjetos(CelObjeto *iniLista);

void  
mostreValor(CelObjeto *pValor);

void  
mostreObjeto(CelObjeto *pObj, int tipoLista);

void  
mostreListaObjetos(CelObjeto *iniLista, int tipoLista);

#endif /* _OBJETOS_H */
