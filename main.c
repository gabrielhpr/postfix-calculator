/*
 * MAC0121 Algoritmos e Estruturas de Dados I
 *
 * NAO E' NECESSARIO EDITAR ESSE MODULO.
 * ENTRETANTO VOCE PODE ALTERA-LO SE ACHAR CONVENIENTE.
 *
 * Aqui esta' a funcao main().
*/

#include <stdio.h>   /* fprintf(), fgets(), printf() */
#include <stdlib.h>  /* exit(), EXIT_FAILURE */
#include <string.h>  /* strncmp */

#include "lexer.h"   /* lexer(), freeFilaItens */
#include "eval.h"    /* itensParaValores(), eval() */
#include "util.h"    /* ITENS, VALORES, Bool, ERRO(), AVISO() */
#include "objetos.h" /* CelObjeto, mostreValor(), mostreObjeto(), 
                        mostreListaObjetos(), freObjeto(), freeListaObjetos() */

#ifdef __linux__
#    define  SYSTEM "linux"
#elif __FreeBSD__
#    define  SYSTEM "FreeBSD"
#elif __MACH__
#    define  SYSTEM "Mac OS X"
#elif __MSDOS__
#    define  SYSTEM "Windows"
#else
#    define  SYSTEM "SO deconhecido..."
#endif

/*---------------------------------------------------------------*/
/* 
 *  C O N S T A N T E S 
 */

/* numero maximo de caracteres em uma linha */
#define MAX_TAMANHO 128 

/*---------------------------------------------------------------*/
/*
 *  P R O T Ó T I P O S
 */

static void 
mostreUso (char *nomePrograma);

/*---------------------------------------------------------------*/
/* 
 *  M A I N 
 */
int 
main(int argc, char *argv[])
{
    char *nomePrograma = argv[0];
    char linha[MAX_TAMANHO]; /* recebe linha do arquivo de entrada */
    
    /* opcoes de execucao e seus valores default */
    Bool modoInterativo      = TRUE;
    Bool mostreItensLexicos  = FALSE;
    Bool mostreValores       = FALSE;
    Bool mostrePilhaExecucao = FALSE;

    FILE *fEntrada   = stdin; 
    char *nomeScript = NULL; /* nome do modo com instrucoes a serem executadas
				 (default = ler do shell) *
			       */
    int nLinha   = -1;  

    /*-------------------------------------------------------------*/
    /* 1 pegue as opcoes da linha de comando */
    while (--argc) 
    {
	if      (!strncmp(argv[argc], "-h", 2)) mostreUso(argv[0]);
	else if (!strncmp(argv[argc], "-i", 2)) modoInterativo      = TRUE;
	else if (!strncmp(argv[argc], "-l", 2)) mostreItensLexicos  = TRUE;
	else if (!strncmp(argv[argc], "-v", 2)) mostreValores       = TRUE;
	else if (!strncmp(argv[argc], "-p", 2)) mostrePilhaExecucao = TRUE;
	else if (!strncmp(argv[argc], "-s", 2))
	{  /* teremos um arquivo com as instrucoes */
	    modoInterativo = FALSE;
	    nomeScript     = argv[argc]+2; 
	}
	else 
	{ /* opcao invalida */
	    fprintf(stderr, "%s: opcao invalida '%s'\n", nomePrograma, argv[argc]); 
	    mostreUso(nomePrograma); 
	}	
    }

    /*-------------------------------------------------------------*/
    /* 2 verifique se ha' um scritp a ser interpretado */
    if (!modoInterativo)
    {  
	fEntrada = fopen(nomeScript, "r");
	if (fEntrada == NULL) 
	{
	    printf("Nao consegui abrir o arquivo '%s'\n", nomeScript);
	    exit(EXIT_FAILURE);
	}
    }

    /*------------------------------------------------------------*/
    /* 3 imprima o cabecalho */
    printf("MAC0121 2019 - EP3\n");
    printf("Pitao (%s, %s)\n", __DATE__, __TIME__);
    printf("[GCC %s] on %s\n", __VERSION__, SYSTEM);

    /* mostre o prompt se o modo e' iterativo */
    if (modoInterativo)
    {
	printf(">>> "); /* prompt */
    }	   

    /*------------------------------------------------------------*/
    /* 4 interprete cada uma das linhas dadas */
    while (fgets(linha, MAX_TAMANHO, fEntrada) != NULL) 
    {
	/* 4.1 fila com a representacao de uma expressao posfixa */
	CelObjeto *iniFila   = NULL; 
	CelObjeto *resultado = NULL; 
	/* 4.2 imprima a linha */
	if (!modoInterativo)
	{
	    printf("Linha %d: %s", ++nLinha, linha);
	}

	/* 4.3 construa a fila de itens lexicos e mostre os itens
               se o programa tiver sido chamado com a opcao "-l" */
	iniFila = crieFilaItens(linha);
	if (iniFila && mostreItensLexicos)
	{
	    /* TAREFA: a funcao a seguir deve ser escrita;
	       veja o modulo objetos.c */
	    mostreListaObjetos(iniFila, ITENS);
	}
	
	/* 4.4 substitua os itens por valores */
	/* TAREFA: a funcao a seguir deve ser escrita;
	   veja o modulo eval.c */
	itensParaValores(iniFila); 
	if (iniFila && mostreValores)
	{
            /* TAREFA: a funcao a seguir deve ser escrita;
               veja o modulo objetos.c */ 
 	    mostreListaObjetos(iniFila, VALORES);
	}
        
	/* 4.5 calcule o valor da expressao posfixa */
	/* TAREFA: a funcao a seguir deve ser escrita;
	   veja o modulo eval.c */
	resultado = eval(iniFila, mostrePilhaExecucao); 
        
	/* 4.6 mostre o resultado/valor da expressao */
	if (resultado)
	{ 
            /* TAREFA: a funcao a seguir deve ser escrita;
               veja o modulo objetos.c */
	    mostreValor(resultado);
	    freeObjeto(resultado);
	}
        
	/* acho que essa pulada de linha e' para algo ficar bonito */ 
	if (!modoInterativo)
	{
	    printf("\n");
	} 
	
	/* 4.7 libere os itens da Fila */
        /*     idealmente so' deveria liberar a celula cabeca */
	/* TAREFA: a funcao a seguir deve ser escrita;
	   veja o modulo util.c */
	freeListaObjetos(iniFila); 
	if (modoInterativo)
	{
	    printf(">>> "); /* prompt */
	}	   
    }
    
    if (modoInterativo)
    {
	printf("\n"); 
    }

    fclose(fEntrada);
    return EXIT_SUCCESS;
}

/*---------------------------------------------------------------*/
/* 
 *  I M P L E M E N T A Ç Ã O   D A S   F U N Ç Õ E S   
 *                 A U X I L I A R E S 
 */

static void 
mostreUso (char *nomePrograma)
{
  fprintf(stderr,"%s: Uso \n"
	  "prompt> %s [-h] [-i | -s<nome-script> ] [-l] [-v]\n"
	  "    -h = mostra como usar o programa\n"
	  "    -i = executa em modo interativo\n"

	  "    -s<nome-script> = calcula as expressões no arquivo <nome-script>\n"
	  "    -l = mostra fila de itens lexicos\n"
	  "    -v = mostra fila de valores\n"
	  "    -p = mostra a pilha de execucao apos cada operacao\n",
	  nomePrograma, nomePrograma);
  exit(EXIT_FAILURE);
}
