#ifndef __DADOS__
#define __DADOS__

/// <summary>
/// =======================================================================
/// Author:         Camila Delarosa
/// Create date:    30/09/2018
/// Description:	Esta biblioteca possue estruturas de dados e suas 
///					respectivas funções de controle desenvolvidas para o
///					programa DNAcrack.
/// =======================================================================
/// </summary>

typedef enum boolean_t
{
	VERDADEIRO,
	FALSO
} boolean_t;

typedef enum tipoGene_t
{
	ADESIVA,
	CEGA,
	NORMAL
} tipoGene_t;

typedef enum tipoCadeia_t
{
	CIRCULAR,
	LINEAR
} tipoCadeia_t;

typedef struct itemCadeia_t
{
	char gene;
	tipoGene_t tipo;
} itemCadeia_t;

typedef struct noCadeiaDNA *ponteiroNoCadeia;
typedef struct noCadeiaDNA
{
	ponteiroNoCadeia anterior;
	itemCadeia_t item;
	ponteiroNoCadeia proximo;
} noCadeiaDNA;

typedef struct cadeiaDNA *ponteiroCadeia;
typedef struct cadeiaDNA
{
	ponteiroNoCadeia primeiro;
	int tamanho;
	tipoCadeia_t tipo;
	ponteiroNoCadeia ultimo;
} cadeiaDNA;

typedef struct noLista *ponteiroNoLista;
typedef struct noLista
{
	ponteiroNoLista anterior;
	cadeiaDNA cadeia;
	ponteiroNoLista proximo;
} noLista;

typedef struct lista
{
	ponteiroNoLista primeiro;
	int numeroNos;
	ponteiroNoLista ultimo;
} lista;

/*-------------------------------------FUNÇÕES--------------------------------------*/

/*CADEIA*/
void cria_cadeia_DNA(cadeiaDNA *c, tipoCadeia_t tipo);

void insere_no_cadeia_DNA(itemCadeia_t i, cadeiaDNA *c);

int verifica_cadeia_vazia(cadeiaDNA c);

void destroi_cadeia(cadeiaDNA *c);

void imprime_cadeia_DNA(cadeiaDNA c); /*criado para teste*/

void imprime_cadeia_DNA_no_arquivo(cadeiaDNA c, FILE *arquivo);

ponteiroNoCadeia procura_ponta_cega(cadeiaDNA c);

void copia_cadeia_DNA_ate_ponta_cega(cadeiaDNA *novaCadeia, cadeiaDNA c, ponteiroNoCadeia pontaCega);

/*LISTA*/
void cria_lista(lista *l);

void insere_no_lista(cadeiaDNA c, lista *l);

int verifica_lista_vazia(lista l);

void exclue_no_lista(ponteiroNoLista p, lista *l);

void imprime_lista(lista l); /*criado para teste*/

void imprime_lista_no_arquivo(lista l, FILE *arquivo);

#endif
