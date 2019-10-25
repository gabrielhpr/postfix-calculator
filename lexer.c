/*
  MAC0121 Algoritmos e Estruturas de Dados I

  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ARQUIVO

  Este arquivo contem a funcao 

       crieFilaItens(linha)

  que e' responsavel pela analise lexica.

  Aqui tambem estao definas as tabelas 

    - operador: contem um string para cada operador, abre e fecha
                parenteses e colchetes;
    - boolean : contem os string "False" e "True" que sao
                as possiveis contantes booleanas.
*/

/*------------------------------------------------------------*/

/* interface para este modulo */
#include "lexer.h"  

/*------------------------------------------------------------*/
#include <stdlib.h> /* malloc(), free() */
#include <stdio.h>  /* printf(), fprintf() */
#include <string.h> /* strlen(), strchr(), strncmp() */
#include <ctype.h>  /* isdigit(), isalpha(), isalnum() */

/*------------------------------------------------------------*/
#include "util.h"       /* mallocSafe(), ERRO(), AVISO() */
#include "objetos.h"    /* celObjetos, freeObjeto(), freeListaObjetos() */
#include "categorias.h" /* MAX_OPERADORES, Categoria, OPE_*, INT, BOOL,...
                         */
/*------------------------------------------------------------*/
/* abreviatura para mensagens de erro de sintaxe              */
/* erros de sintaxe sempre envolvem o string linha            */
#define SYNTAX_ERROR(pos,erroMsg)  \
    fprintf(stderr, "    %s\n", linha);  \
    fprintf(stderr, "    %*c^\n", pos, ' ');  \
    ERRO(crieFileItens em lexer.c: erroMsg);

/*-------------------------------------------------------------*/
/*
  http://en.wikipedia.org/wiki/ASCII

  \t  Horizontal Tab
  \n  Line feed
  \v  Vertical Tab
  \f  Form feed
  \r  Carriage return[g]

Whitespace characters: space, horizontal tab, vertical tab, form feed, newline
http://en.wikipedia.org/wiki/C_language
*/
static const char *whiteSpace = " \t\v\f\n";

/*------------------------------------------------------------*/
/* caracteres usados por operadores */
static const char *carEspecial = "!=><%*/+-_[]()$'";

/*------------------------------------------------------------*/
/* Prototipos de funcoes auxiliares                           */ 

static CelObjeto *
pegueNumero(int *i, char linha[]);

static CelObjeto *
pegueNome(int *i, char linha[]);

static CelObjeto *
pegueString(int *i, char linha[]);

static CelObjeto *
pegueOperador(int *i, char linha[]);

/*-----------------------------------------------------------*/
/* tabela de constantes booleanas a la Python */

char *boolean[2] =
{
    "False", /* 0 */
    "True"   /* 1 */    
};


/*-------------------------------------------------------------*/
/* Tabelas com operadores aritmeticos, relacionais, logicos,
 * indexacao, parenteses e colchetes, tambem a la Python 
 */

/* +4 para ( ) [ ] */
#define MAX_SIMBOLOS  MAX_OPERADORES + 4

char *operador[MAX_SIMBOLOS] =
{
    /* 4 operadores relacionais com 2 simbolos */
    "=="   /*  0 "operador relacional igual" */ 
    ,"!="  /*  1 "operador relacional diferente" */ 
    ,">="  /*  2 "operador relacional maior ou igual" */ 
    ,"<="  /*  3 "operador relacional menor ou igual" */ 

    /* 2 operadores aritmeticos com 2 simbolos */
    ,"**"  /*  4 "operador exponenciacao" */  
    ,"//"  /*  5 "operador divisao inteira" */

    /* 2 operadores relacionais com 1 simbolo */
    ,">"   /*  6 "operador relacional maior" */ 
    ,"<"   /*  7 "operador relacional menor" */ 
    
    /* 6 operadores aritmeticos */
    ,"%"   /*  8 "operador resto de divisao" */ 
    ,"*"   /*  9 "operador aritmetico multiplicacao" */ 
    ,"/"   /* 10 "operador aritmetico divisao" */ 
    ,"+"   /* 11 "operador aritmetico adicao" */ 
    ,"-"   /* 12 "operador aritmetico subtracao" */ 
    ,"_"   /* 13 "operador aritmetico \"menos unario\"" */
    
    /* 3 operadores logicos */
    ,"and" /* 14 "operador logico E "*/  
    ,"or"  /* 15 "operador logico OU"*/ 
    ,"not" /* 16 "operador logico NEGACAO"*/ 
    
    /* operador de indexacao */
    ,"$"   /* 17 "operador indexacao" */

    /* atribuicao... */ 
    ,"="   /* 18 "operador de atribuicao"*/ 

    /* parenteses */                    /*EP4*/
    ,"("   /* 19 "abre  parenteses" */  /*EP4*/
    ,")"   /* 20 "fecha parenteses" */  /*EP4*/

    /* colchetes */
    ,"["   /* 21 "abre colchetes" */    /*IGNORAR*/
    ,"]"   /* 22 "fecha colchetes" */   /*IGNORAR*/
};


/*-------------------------------------------------------------
 * crieFilaItens
 * 
 * RECEBE um string LINHA e RETORNA uma fila com os itens lexicos 
 * na LINHA.
 *
 * IMPLEMENTACAO
 *
 * A fila e' implementada atraves de uma lista ligada COM CABECA.
 * que contem os itens lexicos encontrados a medida que a linha 
 * e' examinada do inicio ao fim.
 * 
 * Cada celula da lista contem a categoria a que o item lexico 
 * pertenece (ver categorias.h) e um string com o item lexico.
 *
 * No caso de operadores e constantes boolean, o string e' um 
 * ponteiro para as tabelas OPERADOR e BOOLEAN, respectivamente.
 * Nao ha alocacao de memoria nesse caso.
 * 
 * Nos demais casos, constantes int, float, cadeia de caracteres
 * e identificadores, ha alocacao de memoria para armazenar
 * o string que representa o item lexico. Essa memoria devera'
 * ser liberada mais tarde, durante a execucao do programa.
 * 
 * Brancos (whitespaces) sao pulados a medida que varremos 
 * a linha e nao fazem parte da lista de itens lexicos.
 *
 * Tudo que esta na LINHA depois de um caractere '#' sera'
 * considerado um comentario e, portanto, sera' ignorado.
 *
 * PRE-CONDICAO 
 *
 *    A funcao assume que a expressao na linha esta' correta.
 *
 *    Apesar de supormos que a expressao esta' correta, 
 *    a funcao retorna NULL se algum problema for encontrado 
 *    na linha.
 *
 * Por conveniencia, esta funcao utiliza a macro 'item' que e' 
 * uma abreviatura para 'valor.pStr' (ver lexer.h).
 */

CelObjeto *
crieFilaItens (char linha[])
{
    CelObjeto *ini = NULL; /* ponteiro para a cabeca da lista/fila */
    CelObjeto *fim = NULL; /* ponteiro para a ultima celula da lista/fila */

    int i; /* indice para percorrer a linha */
    int n = strlen(linha); /* comprimento da linha */

    /* inicialmente a lista de itens esta vazia */
    /* ini e' um ponteiro para a cabeca da fila  */
    ini = mallocSafe(sizeof *ini); /* celula cabeca */
    ini->categoria = INDEFINIDA;  /* indefinida, pois e' celula cabeca */
    ini->item      = NULL; /* NULL, pois e' a cabeca */  
    ini->prox      = NULL; /* NULL, pois a lista esta' vazia */

    /* fim aponta para a ultima celula da lista/fila */     
    fim = ini; 

    i = 0;
    while (i < n) /* equivalente a (*linha != '\0') */
    {
        /* 1 verifique se o caractere e' de comentario */
	if (linha[i] == '#') 
	{  /* encontramos um comentario */
	    i = n; /* acabou o servico do analisador lexico */
	}

	/* 2 verifique se linha[i] e' um branco */
	else if (strchr(whiteSpace, linha[i])) 
	{   
	    i++; /* pule o branco */
	}

        else 
        {  /* 3 encontrarmos um novo item lexico */
            CelObjeto *nova = NULL;

            /* 3.1 verifique se encontramos um numero */
            if (isdigit(linha[i]) || linha[i] == '.') 
            {   
                /* encontramos um numero */
                nova = pegueNumero(&i, linha);
            }

            /* 3.2 verifique se encontramos True, False (BOOL) ou ID (EP4)*/
            /*     foram acrescentados aqui os operadores logicos and, or  */
            /*     e not (Python) */ 
            else if (isalpha(linha[i])) 
            {
                nova = pegueNome(&i, linha);
            }
            
            /* 3.3 verifique se encontramos um string. */
            /*     strings comecam com aspas (") */
            else if (linha[i] == '"')
            {
                nova = pegueString(&i, linha);
            }    
            
	    /* 3.4 verifique se encontramos um operador ou parenteses
                   ou colchetes
	    */
	    else
	    {
                nova = pegueOperador(&i, linha);
            }
            
            /* 3.5 ocorreu algum erro */
            if (nova == NULL) 
            {
                /* Xiii, se chegou aqui teve algum problema */
                freeListaObjetos(ini);
                return NULL; /* abandonamos a linha */
            }

            /* 3.6 esta tudo ok, entao coloque a nova celula na fila */
            else
            {
                fim->prox = nova;
                fim = nova;
            }
        }
    }

    return ini;
}
    
/*--------------------------------------------------------*/
/* 
 * pegueNumero
 *
 * RECEBE um indice *I de um string LINHA tal que LINHA[*I] 
 * e' o primeiro caractere de um numero (um digito ou um '.').
 *
 * RETORNA o endereco de uma nova CelObjeto que contem 
 * no campo: 
 *
 *     - categoria : o tipo do numero;
 *     - item      : um ponteiro para um string que e' copia 
 *                   do numero que comeca em LINHA[*I];
 *     - prox      : o valor NULL.
 *
 * Se algum erro for detectado a funcao retorna NULL.
 * Em caso contrario, a funcao DEVOLVE em *I o indice da 
 * primeira posicao apos o numero.
 */

static CelObjeto *
pegueNumero(int *i, char linha[])
{
    CelObjeto *nova = mallocSafe(sizeof *nova);
    int k = *i;    
    int j;

    nova->categoria = INDEFINIDA;
    nova->item      = NULL;
    nova->prox      = NULL;

    /* anda enquanto e' digito */
    for (j = 0; linha[k+j] && isdigit(linha[k+j]); j++) ;

    /* achamos um float ou um int ? */
    if (linha[k+j] == '.')
    {
        nova->categoria = FLOAT_STR;
        /* anda sobre o resto do float */
        for (j++ ; linha[k+j] && isdigit(linha[k+j]); j++) ;
    }
    else
    {
        nova->categoria = INT_STR;
    }
 
    /* se nao for um branco nem um caractere especial... erro */
    if (! ( strchr(whiteSpace,  linha[k+j]) || 
            strchr(carEspecial, linha[k+j]) ) )
    { 
        /* mensagem copiada do interpretador Python */
        SYNTAX_ERROR(k+j,SyntaxError: invalid syntax);
        return NULL; /* avisamos que ocorreu um erro */
    }    

    /* aloque a area para o string: +1 para o '\0' */ 
    nova->item = mallocSafe((j+1)*sizeof(char));

    /* copie os j caracteres para o campo item da celula */
    strncpy(nova->item, &linha[k], j);

    /* coloque um '\0' no final para termos de fato um string */
    nova->item[j] = '\0';

    *i = k + j;
    return nova;
}


/*--------------------------------------------------------*/
/* 
 * pegueNome
 *
 * RECEBE um indice *I de um string LINHA tal que
 * LINHA[*I] e' uma letra.
 *
 * RETORNA o endereco de uma nova CelObjeto que contem 
 * no campo: 
 *
 *     - categoria : o tipo do nome encontrado (BOOL ou ID
 *                   ou OPER_LOGICO_* );
 *     - item      : um ponteiro para um string
 *                   na tabela boolean (categoria == BOOL) ou 
 *                   na tabela operador (categoria == OPER_LOGICO_*) 
 *                   com o nome   (categoria == ID);
 *     - prox      : o valor NULL.
 *
 *  Alem disso, a funcao DEVOLVE em *I o indice da primeira
 *  posicao apos o numero.
 */
static CelObjeto *
pegueNome(int *i, char linha[])
{
    CelObjeto *nova = mallocSafe(sizeof *nova);
    int k = *i;    
    int j;
    char *str;

    nova->categoria = INDEFINIDA;
    nova->item      = NULL;
    nova->prox      = NULL;

    /* anda enquanto for uma letra ou digito */
    for (j = 0; linha[k+j] && isalnum(linha[k+j]); j++) ;
    
    /* aloque a area para o string: +1 para o '\0' */ 
    str = mallocSafe((j+1)*sizeof(char));

    /* copie os j caracteres para o campo item da celula */
    /* The strncpy() function is similar to strcpy, 
       except that at most n bytes of src are copied.
       
       Warning: If there is no null byte among the first 
       n bytes of src, the string placed in dest will not be
       null-terminated.
    */
    strncpy(str, &linha[k], j);

    /* coloque um '\0' no final para termos de fato um string */
    str[j] = '\0';

    if (!strcmp(str, "False"))  
    {
        nova->categoria  = BOOL_STR;
        nova->item = boolean[FALSE];
    } 
    else if (!strcmp(str, "True"))
    {
        nova->categoria  = BOOL_STR;
        nova->item = boolean[TRUE];
    }
    /* Hmmm.  O trecho a seguir foi inserido para tratar dos 
       operadores logicos and, or e not. Nao precisaria disso
       se usassemos &&, || e ! ... 
    */
    else if (!strcmp(str, "and"))
    {
        nova->categoria  = OPER_LOGICO_AND;
        nova->item = operador[OPER_LOGICO_AND];
    }
    else if (!strcmp(str, "or"))
    {
        nova->categoria  = OPER_LOGICO_OR;
        nova->item = operador[OPER_LOGICO_OR];
    }
    else if (!strcmp(str, "not"))
    {
        nova->categoria  = OPER_LOGICO_NOT;
        nova->item = operador[OPER_LOGICO_NOT];
    }
    /* 
       termina aqui o trecho inserido por causa do and, or, not 
    */
    else 
    { 
        /* encontramos um identificador */
        nova->categoria = ID;

        /* coloque o identificador na celula */ 
        nova->item = str;
    }

    /* libere a area do string caso nao esteja em uso */
    if (nova->categoria != ID) 
    {
        free(str);
    }

    *i = k + j;
    return nova;
}

/*--------------------------------------------------------*/
/* 
 * pegueString
 *
 * RECEBE um indice *I de uma string LINHA tal que
 * LINHA[*I] == '"'.
 *
 * RETORNA o endereco de uma nova CelObjeto que contem 
 * no campo: 
 *
 *     - categoria : o valor STR;
 *     - item      : um ponteiro para uma copia do string 
 *                   que comeca em LINHA[*I+1];
 *     - prox      : o valor NULL.
 *
 *  Alem disso, a funcao DEVOLVE em *I o indice da primeira
 *  posicao apos o string.
 *
 */

static CelObjeto *
pegueString(int *i, char linha[])
{
    CelObjeto *nova = mallocSafe(sizeof *nova);
    int k = *i+1;    
    int j;

    nova->categoria = INDEFINIDA;
    nova->item      = NULL;
    nova->prox   = NULL;
    
    /* anda enquanto nao for um fecha string */
    for (j = 0; linha[k+j] && linha[k+j] != linha[*i]; j++) ;

    if (linha[k+j] != linha[*i])
    {
        /* mensagem copiada do interpretador Python */
        SYNTAX_ERROR(k+j,SyntaxError: EOL while scanning string literal);
        freeObjeto(nova); /* libera nova celula */
        return NULL; /* avisamos que ocorreu um erro */
    }
    else 
    { 
        nova->categoria = STR;

        /* aloque a area para o string: +1 para o '\0' */ 
        nova->item = mallocSafe((j+1)*sizeof(char));
        
        /* copie os j caracteres para o campo item da celula */
        strncpy(nova->item, &linha[k], j);

        /* coloque um '\0' no final para termos de fato um string */
        nova->item[j] = '\0';
    }


    *i = k + j + 1; /* +1 para pular o '"' */
    return nova;
}

/*--------------------------------------------------------*/
/* 
 * pegueOperador
 *
 * RECEBE um indice *I de uma string LINHA.
 *
 * Se a funcao encontra LINHA[*I..] na tabela operador,
 * entao RETORNA o endereco de uma nova CelObjeto
 * que contem no campo: 
 *
 *     - categoria : o tipo do operador;
 *     - item: um ponteiro para um string na tabela 
 *                   operador;
 *     - prox   : o valor NULL.
 *
 *  Se algum erro for detectado, a funcao retorna NULL.
 *  Caso contrario, a funcao DEVOLVE em *I o indice da 
 *  primeira posicao apos o numero.
 */

static CelObjeto *
pegueOperador(int *i, char linha[])
{
    CelObjeto *nova = mallocSafe(sizeof *nova);
    int k = *i;    
    int j;

    nova->categoria = INDEFINIDA;
    nova->item      = NULL;
    nova->prox      = NULL;

    /* procure por linha[i..] na tabela de operadores */
    j = 0;
    while (j < MAX_SIMBOLOS 
           && strncmp(&linha[k], operador[j], strlen(operador[j])))
    {
        j++;
    }
	 
    if (j == MAX_SIMBOLOS) 
    {   /* hmmm... nao encontrou o operador... erro */
        /* mensagem copiada do interpretador Python */
        SYNTAX_ERROR(k,SyntaxError: invalid syntax);
        freeObjeto(nova);
        return NULL;
    }

    /* encontrou linha[i..] na tabela de operadores? */
    nova->categoria  = j;
    nova->item = operador[j];
        
    /* atualize o ponteiro para a linha */
    *i += strlen(operador[j]);

    return nova;
}
