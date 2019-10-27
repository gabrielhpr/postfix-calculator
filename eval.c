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

  eval.c
  Pitao I

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerado plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* interface para as funcoes deste modulo */
#include "eval.h" 

/*------------------------------------------------------------*/
#include <stdlib.h>  /* atoi(), atof() */
#include <string.h>  /* strncmp(), strlen(), strncpy(), strcpy(), strcmp() */
#include <math.h>    /* pow() */

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES */
#include "util.h"       /* ERRO(), AVISO(), mallocSafe() */
#include "objetos.h"    /* CelObjeto, mostreObjeto(), freeObjeto()
                           mostreListaObjetos(), 
                           freeListaObjetos() */

#include "stack.h"      /* funcoes que manipulam uma pilha */ 
                        /* essa pilha sera usada para implementar 
                           a pilha de execucao */


/*------------------------------------------------------------*/
/* Protipos de funcoes auxiliares */

/*------------------------------------------------------------*/
/* Tabela com uma representacao da precedencia dos operadores
 * atraves de valores inteiros. 
 * Quanto maior o valor, maior a precedencia.
 *
 *  http://www.ime.usp.br/~pf/algoritmos/apend/precedence.html
 */
static const int precedencia[MAX_OPERADORES] =
{
    /* 4 operadores relacionais com 2 simbolos  */
     4   /* "==" */ /* da esquerda para a direita */
    ,4   /* "!=" */ /* da esquerda para a direita */
    ,5   /* ">=" */ /* da esquerda para a direita */
    ,5   /* "<=" */ /* da esquerda para a direita */
    
    /* 2 operadores aritmeticos com 2 simbolos */
    ,8 /* ,"**" */ /* da direita para a esquerda */
    ,7 /* ,"//" */ /* da esquerda para a direita */
 
    /* 2 operadores relacionais com 1 simbolo */
    ,5  /* ">"  */ /* da esquerda para a direita */
    ,5  /* "<"  */ /* da esquerda para a direita */ 
    
    /* 6 operadores aritmeticos */
    ,7  /* "%" */ /* da esquerda para a direita */
    ,7  /* "*" */ /* da esquerda para a direita */
    ,7  /* "/" */ /* da esquerda para a direita */
    ,6  /* "+" */ /* da esquerda para a direita */
    ,6  /* "-" */ /* da esquerda para a direita */
    ,8  /* "_" */ /* da direita para a esquerda */
    
    /* 3 operadores logicos  */
    ,3  /* "and" */ /* da esquerda para a direita */ 
    ,2  /* "or" */ /* da esquerda para a direita */
    ,8  /* "not"  */ /* da direita para a esquerda */
    
    /* operador de indexacao */
    ,9  /* "$"  */ /* da esquerda para a direita (IGNORAR)*/

    /* atribuicao */ 
    ,1  /* "=" */ /* da direita para a esquerda EP4 */ 
}; 

/*-------------------------------------------------------------
 *  itensParaValores
 *  
 *  RECEBE uma lista ligada com cabeca INIFILAITENS representando uma
 *  fila de itens lexicos. Inicialmente, o campo VALOR de cada celula
 *  contem um string com o item lexico.
 *
 *  A funcao CONVERTE o campo VALOR de cada celula para um inteiro 
 *  ou double, como descrito a seguir. 
 *
 *  IMPLEMENTACAO
 *
 *  O campo VALOR de uma CelObjeto tem 3 subcampos:
 *
 *      - valor.vInt   (int)
 *      - valor.vFloat (float)
 *      - valor.pStr   (string)
 *
 *  Nessa conversao, o campo VALOR de cada celula recebe um valor 
 *  da seguinte maneira.
 *
 *     - Se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um int (INT_STR) entao o campo 
 *       valor.vFloat deve receber esse numero inteiro. 
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um float (FLOAT_STR) entao o campo 
 *       valor.vFloat deve receber esse float;
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um Bool (BOOL_STR) entao o campo 
 *       valor.vFloat deve receber o inteiro correspondente 
 *       (False = 0, True = 1);
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e' um
 *       string representando um operador (OPER_*) entao o campo 
 *       valor.vInt deve receber o inteiro correspondente 
 *       a precedencia desse operador. 
 *       
 *       Para isto utilize o vetor PRECEDENCIA declarado antes 
 *       desta funcao. 
 *
 * Nesta funcao (e nas outras) voce pode utilizar qualquer funcao da
 * biblioteca string do C, como, por exemplo, atoi(), atof().
 *
 * Esta funcao (e todas as outras) devem 'dar' free nas estruturas que
 * deixarem de ser necessarias.
 *
 * Esse e o caso dos strings dos itens das categorias FLOAT_STR e INT_STR.  
 *
 * Os campos strings de objetos OPER_* e BOOL_STR apontam para
 * strings em tabelas definidas no modulo lexer.h.  Nesse
 * caso, tentar liberar essa memoria e' um erro.
 *
 */

void
itensParaValores(CelObjeto *iniFilaItens) {
    CelObjeto* pontAuxiliar = iniFilaItens->prox;
    double auxiliar;
    while (pontAuxiliar != NULL) {
        if(pontAuxiliar->categoria == INT_STR) {
            /*Transforma a string de valor.pStr, para um float em valor.vFloat*/
            auxiliar = atof(pontAuxiliar->valor.pStr);
            /*Libera a string INT_STR*/
            free(pontAuxiliar->valor.pStr);
            pontAuxiliar->valor.vFloat = auxiliar;
            /*Altera a categoria do CelObjeto*/
            pontAuxiliar->categoria = FLOAT;
            /*free(pontAuxiliar->valor.pStr);*/
        }
        else if(pontAuxiliar->categoria == FLOAT_STR) {
            auxiliar = atof(pontAuxiliar->valor.pStr);
            free(pontAuxiliar->valor.pStr);
            pontAuxiliar->valor.vFloat = auxiliar;
            pontAuxiliar->categoria = FLOAT;
        }
        else if(pontAuxiliar->categoria == BOOL_STR) {
            auxiliar = atof(pontAuxiliar->valor.pStr);
            pontAuxiliar->valor.vFloat = auxiliar;
            pontAuxiliar->categoria = FLOAT;
        }
        /*Os OPERADORES pertencem ao intervalo [0,18], da lista de categorias*/
        else if(pontAuxiliar->categoria >= 0 && pontAuxiliar->categoria <= 18) {
            pontAuxiliar->valor.vInt = precedencia[pontAuxiliar->categoria];
        }
        /*Vai para o próximo CelObjeto*/
        pontAuxiliar = pontAuxiliar->prox;
    } 
}

/*-------------------------------------------------------------
 * eval
 * 
 * RECEBE uma lista ligada como cabeca INIPOSFIXA que representa
 * uma expressao em notacao posfixa. 
 *
 * RETORNA o endereco de uma celula com o valor da expressao.
 *
 * PRE-CONDICAO: a funcao supoe que a expressao esta' sintaticamente
 *               correta.
 *
 * IMPLEMENTACAO
 *
 * Para o calculo do valor da expressao, deve ser usada uma pilha. 
 * O endereco retornado sera' o da celula no topo no topo dessa  
 * "pilha de execucao".
 * 
 * A funcao percorre a expressao calculando os valores resultantes.
 * Para isto e' utilizada uma pilha de execucao. 
 * 
 * A implementacao das funcoes que manipulam a pilha devem ser
 * escritas no modulo stack.c.
 * 
 * O arquivo stack.h contem a interface para essas funcoes. 
 * A pilha de execucao so deve ser acessada atraves dessa interface
 * (caso contrario nao chamariamos stack.h de interface).
 * 
 * O parametro mostrePilhaExecucao indica se os valores
 * na pilha de execucao devem ser exibidos depois de qualquer 
 * alteracao.
 * 
 * Esta funcao deve "dar free" nas estruturas que deixarem de ser 
 * necessarias.
 *
 */

CelObjeto *
eval (CelObjeto *iniPosfixa, Bool mostrePilhaExecucao) {
    /*A lista tem cabeça*/
    CelObjeto* pontAuxFila = iniPosfixa->prox;
    /*Inicializa a pilha*/
    Stack s = stackInit();

    CelObjeto* obj;
    double a,b,resul;
    resul = 0;

    while(pontAuxFila != NULL) {
        /*É um valor*/
        if(pontAuxFila->categoria == FLOAT) {
            /*Empilha o valor*/
            stackPush(s, pontAuxFila);
            /*Exibe a pilha de execução*/
            if(mostrePilhaExecucao == TRUE)
                stackDump(s);
        }
        /*É um operador*/
        else if(pontAuxFila->categoria >= 0 && pontAuxFila->categoria <= 18) {
            /*Operadores de dois operandos*/
            if(pontAuxFila->categoria != 13 && pontAuxFila->categoria < 16) {

                /*Pega o elemento do topo*/
                obj = stackPop(s);
                a = obj->valor.vFloat;
                /*Libera o CelObjeto do primeiro operando, pois é possível armazenar
                o valor da operação no segundo operando*/
                freeObjeto(obj);
                if(mostrePilhaExecucao == TRUE)
                    stackDump(s);
                    
                /*Pega o segundo elemento do topo*/
                obj = stackPop(s);
                b = obj->valor.vFloat;
                if(mostrePilhaExecucao == TRUE)
                    stackDump(s);

                switch(pontAuxFila->categoria) {
                    case(OPER_IGUAL):
                        resul = a == b;
                        break;
                    case(OPER_DIFERENTE):
                        resul = a != b;
                        break;
                    case(OPER_MAIOR_IGUAL):
                        resul = b >= a;
                        break;
                    case(OPER_MENOR_IGUAL):
                        resul = b <= a;
                        break;
                    case(OPER_EXPONENCIACAO):
                        resul = pow(b,a);
                        break;
                    case(OPER_DIVISAO_INT):
                        resul = (int) b/ (int)a;
                        break;
                    case(OPER_MAIOR):
                        resul = b > a;
                        break;
                    case(OPER_MENOR):
                        resul = b < a;
                        break;
                    case(OPER_RESTO_DIVISAO):
                        resul = fmod(b,a);
                        break;
                    case(OPER_MULTIPLICACAO):
                        resul = a * b;
                        break;
                    case(OPER_DIVISAO):
                        resul = b / a;
                        break;
                    case(OPER_ADICAO):
                        resul = a + b;
                        break;
                    case(OPER_SUBTRACAO):
                        resul = (b - a);
                        break;
                    case(OPER_LOGICO_AND):
                        resul = (b && a);
                        break;
                    case(OPER_LOGICO_OR):
                        resul = (b || a);
                        break;
                    default:
                        printf("Esse default evita os warnings\n");
                }
                /*Atribui o resultado da operação ao CelObjeto*/
                obj->valor.vFloat = resul;             
                /*Empilha o resultado*/       
                stackPush(s, obj); 
                freeObjeto(obj);

                if(mostrePilhaExecucao == TRUE)
                    stackDump(s); 
            }
            /*Operador unário*/
            else if (pontAuxFila->categoria == 13 || pontAuxFila->categoria == 16) {
                /*Menos(-) unário*/
                /*Pega o elemento do topo*/
                obj = stackPop(s);
                if(mostrePilhaExecucao == TRUE)
                    stackDump(s);
                a = obj->valor.vFloat;

                /*Decrementa o valor de a*/
                if(pontAuxFila->categoria == 13)
                    a = a*(-1);
                /*Nega o a*/
                else
                    a = !a;
                /*Atualiza o valor do CelObjeto*/
                obj->valor.vFloat = a;
                /*Devolve o resultado para a pilha*/
                stackPush(s, obj);
                freeObjeto(obj);
                if(mostrePilhaExecucao == TRUE)
                    stackDump(s);
            }
        }
        pontAuxFila = pontAuxFila->prox;
    }
    /*Obtém o resultado das operações*/
    obj = stackTop(s);
    /*Libera a cabeça da pilha*/
    freeObjeto(s);
    return obj;
}
