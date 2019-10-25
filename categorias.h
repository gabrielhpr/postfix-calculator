/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO
 * 
 * Neste arquivo esta' a declaraco do tipo Categoria alem de
 * uma tabela com as constantes que representam as possiveis
 * categorias dos itens lexicos.
 *
 */

#ifndef _CATEGORIAS_H
#define _CATEGORIAS_H

/*-------------------------------------------------------------*/
/* 
 *  C O N S T A N T E S 
 */

/* 
 * numero de operadores
 * aritmeticos + relacionais + logicos + indexacao + atribuicao 
 *    8        +      6      +    3    +     1     +     1
 */               
#define MAX_OPERADORES 19

/*-------------------------------------------------------------*/
/* temos 32 categorias de itens lexicos                        */
/* sendo 19 para operadores, 4 para o abre e fecha parenteses  */
/* e colchetes (ignorar colchetes), 3 para strings, 4 para     */
/* constantes e uma para um identificador (EP4) e uma para     */
/* indefinida */
#define MAX_CATEGORIAS  32

enum listaCategorias
{
    /* 4 operadores relacionais com 2 simbolos  */
    /*                   simbolo, constante     */ 
    OPER_IGUAL           /* "==",  0 */
    ,OPER_DIFERENTE      /* "!=",  1 */
    ,OPER_MAIOR_IGUAL    /* ">=",  2 */
    ,OPER_MENOR_IGUAL    /* "<=",  3 */
    
    /* 2 operadores aritmeticos com 2 simbolos */
    ,OPER_EXPONENCIACAO  /* "**",  4 */
    ,OPER_DIVISAO_INT    /* "//",  5 */

    /* 2 operadores relacionais com 1 simbolo */
    ,OPER_MAIOR          /* ">",   6  */
    ,OPER_MENOR          /* "<",   7  */  
    
    /* 6 operadores aritmeticos */
    ,OPER_RESTO_DIVISAO  /* "%",   8 */
    ,OPER_MULTIPLICACAO  /* "*",   9 */
    ,OPER_DIVISAO        /* "/",  10 */
    ,OPER_ADICAO         /* "+",  11 */
    ,OPER_SUBTRACAO      /* "-",  12 */ 
    ,OPER_MENOS_UNARIO   /* "_",  13 */  
    
    /* 3 operadores logicos */
    ,OPER_LOGICO_AND     /* "and",14 */ 
    ,OPER_LOGICO_OR      /* "or", 15 */
    ,OPER_LOGICO_NOT     /* "not",16 */
    
    /* operador de indexacao */
    ,OPER_INDEXACAO     /* "$" ,  17 */ /*IGNORAR*/
    
    /* atribuicao */ 
    ,OPER_ATRIBUICAO    /* "=" ,  18 */   /*EP4*/

    /* parenteses: para expressoes infixas */
    ,ABRE_PARENTESES     /* "(" , 19 */  /*EP4*/
    ,FECHA_PARENTESES    /* ")" , 20 */  /*EP4*/
    
    /* colchetes: para indices de strings */
    ,ABRE_COLCHETES      /* "[" , 21 */  /*IGNORAR*/
    ,FECHA_COLCHETES     /* "]" , 22 */  /*IGNORAR*/

    /* strings representando constantes */
    ,BOOL_STR           /*       23 */
    ,FLOAT_STR          /*       24 */
    ,INT_STR            /*       25 */

    /* categorias das constantes */
    ,STR                /*       26 */   /*IGNORAR*/
    ,BOOL               /*       27 */
    ,FLOAT              /*       28 */
    ,INT                /*       29 */
     
    /* identificador */ 
    ,ID          /* nome do identificador, 30 */ /*EP4*/ 
    
    /* categoria indefinida */
    ,INDEFINIDA  /* 31 */
};
/* warning: ISO C forbids forward references to ‘enum’ types [-pedantic] */
typedef enum listaCategorias Categoria; 

#endif /* _CATEGORIAS_H */
