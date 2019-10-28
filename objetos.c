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

  objetos.c
  Pitao I

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.

  Exemplo:
  - função mallocc retirada de: 

    http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdlib.h>  /* exit(), malloc(), EXIT_FAILURE */ 
#include <stdio.h>   /* printf(), stdout */

#include "util.h"       /* AVISO(), FILA_ITENS */
#include "categorias.h" /* MAX_CATEGORIAS, OPER_*,  */
#include "objetos.h"    /* CelObjeto, freeObjeto(), freeListaObjetos() 
                           mostreObjeto(), mostreListaObjetos() */ 


/*-----------------------------------------------------------*/
/* OBSERVACAO
 *
 * A ordem  dos strings a seguir e' compativel com a ordem das
 * tabelas de operadores (lexer.c) e com a enumeracao das
 * categorias (categorias.h).
 *
 * A tabela a seguir e' usada para exibir uma descricao da
 * categoria dos itens lexicos e valores.
 */

static char *boolean[2] = { "False", "True" };

static char *nomeCategoria[MAX_CATEGORIAS] =
{
    /* 4 operadores relacionais com 2 simbolos */
    /* "==", 0 */  "operador relacional igual"
    /* "!=", 1 */ ,"operador relacional diferente"
    /* ">=", 2 */ ,"operador relacional maior ou igual"
    /* "<=", 3 */ ,"operador relacional menor ou igual"
 
    /* 2 operadores aritmeticos com 2 simbolos */
    /* "**", 4  */ ,"operador aritmetico exponenciacao" 
    /* "//", 5  */ ,"operador aritmetico divisao inteira"
 
    /* 2 operadores relacionais com 1 simbolo */
    /* ">" , 6  */ ,"operador relacional maior"
    /* "<" , 7  */ ,"operador relacional menor"

    /* 6 operadores aritmeticos com 1 simbolo */
    /* "%" , 8  */ ,"operador aritmetico resto de divisao"
    /* "*" , 9  */ ,"operador aritmetico multiplicacao"
    /* "/" ,10  */ ,"operador aritmetico divisao"
    /* "+" ,11  */ ,"operador aritmetico adicao"
    /* "-" ,12  */ ,"operador aritmetico subtracao"
    /* "_" ,13  */ ,"operador aritmetico \"menos unario\""

    /* 3 operadores logicos */
    /* "and",14 */ ,"operador logico e" 
    /* "or",15 */ ,"operador logico ou"
    /* "not" ,16 */ ,"operador logico negacao"

    /* operador de indexacao */
    /* "$" ,17 */ ,"operador indexacao" /*IGNORAR*/

    /* atribuicao */ 
    /* "=" ,18 */  ,"operador de atribuicao"  /*EP4*/

    /* EP4: parenteses */ 
    /* "(" ,19 */  ,"abre parenteses"     /*EP4*/
    /* ")" ,20 */  ,"fecha parenteses"    /*EP4*/

    /* colchetes para listas */
    /* "[" ,21 */  ,"abre colchetes"      /*IGNORAR*/
    /* "]" ,22 */  ,"fecha colchetes"     /*IGNORAR*/

    /* constantes */
    /* 23 */      ,"string representando bool"
    /* 24 */      ,"string representando float"
    /* 25 */      ,"string representando int"

    /* PiObjetos */
    /* 26 */      ,"valor str"
    /* 27 */      ,"valor bool"
    /* 28 */      ,"valor float"
    /* 29 */      ,"valor int"

    /* identificador */
    /* 30 */ ,"nome de um identificador"  /*EP4*/
    /* 31 */ ,"indefinida" 
};


/*-------------------------------------------------------------
 * freeObjeto
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto 
 * e libera a area alocada por esta celula. 
 *
 * Se o campo valor contem um ponteiro para um string, entao 
 * a area alocada para o string tambem e' liberada. 
 * Isso deve ser feito apenas quando a categoria do objeto for
 * FLOAT_STR, INT_STR, STR (IGNORAR) e ID.
 *
 * ATENCAO:
 * 
 *     Para chamar essa funcao e' importante que o campo categoria
 *     da celula tenho sido inicializado apropriadamente.
 * 
 *     Isso e' importante para liberar a memoria de objetos que
 *     nao sao inteiramente alocados na CelObjeto. No caso deste
 *     EP somente strings nao sao inteiramente alocados na
 *     CelObjeto (FLOAT_STR, INT_STR, ...).
 */

void
freeObjeto(CelObjeto *pObj) {
    if (pObj->categoria == FLOAT_STR || pObj->categoria == INT_STR || pObj->categoria == ID) {
        /*Libera a string*/
        free(pObj->valor.pStr);
    }
    /*Libera o celObjeto*/
    free(pObj);    
}

/*-------------------------------------------------------------
 * freeListaObjetos(iniLista)
 *
 * Recebe uma lista INILISTA de celulas do tipo CelObjeto e 
 * libera toda a memoria alocada para lista. 
 *
 * Esta funcao deve utilizar a funcao freeObjeto().
 *
 */

void
freeListaObjetos(CelObjeto *iniLista) {
    /*Ponteiro auxiliar para CelObjeto*/
    CelObjeto* pontLista = iniLista;

    while(iniLista != NULL) {
        pontLista = iniLista->prox;
        freeObjeto(iniLista);
        iniLista = pontLista;
    }
}

/*-------------------------------------------------------------
 * mostreValor
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto e
 * imprime o conteudo do seu campo VALOR.
 *
 * PRE-CONDICAO: 
 * 
 *     Aa funcao supoe que o campo valor contem uma constante.
 *     No caso deste EP podemos supor que a categoria de uma
 *     constante e' FLOAT (representada por um double).  Nesse
 *     caso a funcao e' praticamente um mero printf.
 *
 * OBSERVACAO:
 * 
 *     Em geral, uma constante poderia ser da categoria FLOAT,
 *     INT (ignorada), BOOL (ignorada) ou STR (ignorada).
 */

void
mostreValor(CelObjeto *pValor) {
    if(pValor->categoria == FLOAT) {
        printf("%f\n", pValor->valor.vFloat);
    }
}

/*-------------------------------------------------------------
 * mostreObjeto
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto 
 * e um inteiro TIPOLISTA. 
 * 
 * A funcao imprime o conteudo do campo VALOR da celula pOBJ e
 * uma descricao de sua categoria.  A descricao da categoria esta'
 * na tabela nomeCategoria (definida acima).
 *
 * O que deve ser impresso depende do valor de TIPOLISTA, que
 * pode ser ITENS ou VALORES (util.h):
 *
 *   - tipoLista == ITENS
 *
 *     O campo valor.pStr da celula contem um string com um 
 *     item lexico.
 *
 *     Exemplos de impressoes feitas pela funcao nesse caso:
 *
 *         "2.0"  (string representando float)
 *         "+"  (operador aritmetico adicao)
 *         "**"  (operador aritmetico exponenciacao)
 *
 *   - tipoLista == VALORES
 *      
 *     Se a categoria for de um OPER_*, o campo valor.vInt contem
 *     um inteiro representando a precedencia (eval.c) do operador.
 *
 *     Se a categoria for FLOAT, o campo valor.vFloat contem 
 *     um double.
 *      
 *     Exemplos de impressoes feitas pela funcao nesse caso: 
 *
 *         2.000000  (valor float)
 *         3.140000  (valor float)
 *         4.000000  (valor float)
 *         prec=6  (operador aritmetico adicao)
 *         prec=8  (operador aritmetico exponenciacao)
 *
 * A descricao da categoria da celula esta' em  
 * 
 *     nomeCategoria[pObj->categoria])
 *
 * 
 * OBSERVACAO. 
 * 
 * Em geral, essa funcao consideraria outras categorias que estao
 * sendo ignoradas no EP.
 *
 */
	
void
mostreObjeto(CelObjeto *pObj, int tipoLista) {
    if(tipoLista == ITENS){
        printf(" '%s' ", pObj->valor.pStr);
        printf(" (%s) \n", nomeCategoria[pObj->categoria]);
    }
    else if (tipoLista == VALORES) {
        if(pObj->categoria == FLOAT) {
            printf(" %f ", pObj->valor.vFloat);
            printf(" (%s) \n", nomeCategoria[pObj->categoria]);
        }
        /*Os enums de Operações possuem valores no intervalo [0,18]*/
        else if(pObj->categoria >= 0 && pObj->categoria <= 18) {
            printf(" prec=%d ", pObj->valor.vInt);
            printf(" (%s) \n", nomeCategoria[pObj->categoria]);
        }
    }    
}

/*-------------------------------------------------------------
 *  mostreListaObjetos
 *
 * Recebe uma lista encadeada com cabeca INILISTA de celulas 
 * do tipo CelObjeto e um inteiro TIPOLISTA.
 *
 * A funcao imprime um cabecalho e o conteudo do campo VALOR de
 * cada celula da lista, junto com uma descricao desse valor.
 *
 * O parametro TIPOLISTA indica o conteudo das celulas.
 * Temos a seguinte relacao "TIPOLISTA x conteudo da celula":
 *
 *   tipo          categoria     campo VALOR contem
 *   ----------------------------------------------  
 *   ITENS         todas         string 
 *   VALORES       OPER_*        int representando a precedencia   
 *   VALORES       FLOAT         double  
 *   VALORES       INT           int  (IGNORAR)
 *   VALORES       BOOL          int representando True e False (IGNORAR)
 *   VALORES       STR           string (IGNORAR)
 *
 * Exemplos de impressao da funcao para TIPOLISTA == ITENS
 * 
 *     ==========================
 *      Fila de itens lexicos  
 *      item  (categoria)
 *     . . . . . . . . . . . . .
 *     "True"  (string representando bool)
 *     "1"  (string representando int)
 *     "+"  (operador aritmetico adicao)
 *     "2.0"  (string representando float)
 * 
 * Exemplos de impressao da funcao para TIPOLISTA == VALORES
 *
 *     ==========================
 *       Fila de valores   
 *       valor  (categoria)
 *     . . . . . . . . . . . . . .
 *     2.000000  (valor float)
 *     3.140000  (valor float)
 *     4.000000  (valor float)
 *     prec=6  (operador aritmetico adicao)
 *     prec=8  (operador aritmetico exponenciacao)
 *     
 */ 

void
mostreListaObjetos (CelObjeto *iniLista, int tipoLista) {
    CelObjeto* pontAux = iniLista->prox;
    if(tipoLista == ITENS) {
        printf("==================\n");
        printf("Fila de itens lexicos\n");
        printf("item (categoria)\n");
        printf(". . . . . . . . . .\n");
        while (pontAux != NULL) {
            mostreObjeto(pontAux, tipoLista);
            pontAux = pontAux->prox;
        }
        printf("\n");
    }
    else if (tipoLista == VALORES) {
        printf("==================\n");
        printf("Fila de valores\n");
        printf("valor (categoria)\n");
        printf(". . . . . . . . . .\n");
        while (pontAux != NULL) {
            mostreObjeto(pontAux, tipoLista);
            pontAux = pontAux->prox;
        }
        printf("\n");
    }
}    
