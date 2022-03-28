#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Dados.h"

/*CADEIA*/
void cria_cadeia_DNA(cadeiaDNA *c, tipoCadeia_t tipo)
{
	// converte o ponteiro de retorno do malloc, para um ponteiro do tipo Nó de cadeia
	c->primeiro = (ponteiroNoCadeia) malloc(sizeof(noCadeiaDNA));
	c->ultimo = c->primeiro;

	if (tipo == LINEAR) {
		c->primeiro->anterior = NULL;
		c->primeiro->proximo = NULL;
		c->tipo = LINEAR;
	}
	else {
		c->primeiro->anterior = c->ultimo;
		c->ultimo->proximo = c->ultimo;
		c->tipo = CIRCULAR;
	}
	c->tamanho = 0;
}

void insere_no_cadeia_DNA(itemCadeia_t i, cadeiaDNA *c)
{
	ponteiroNoCadeia aux = c->ultimo; //armazena endereço do ultimo atual

	c->ultimo->proximo = (ponteiroNoCadeia) malloc(sizeof(noCadeiaDNA)); //cria novo nó
	c->ultimo = c->ultimo->proximo; //atualiza o ultimo
	c->ultimo->item = i; //insere item
	c->ultimo->anterior = aux; //salva o endereço do 'ultimo' anterior
	
	if (c->tipo == LINEAR)
		c->ultimo->proximo = NULL;
	else {
		c->ultimo->proximo = c->primeiro;
		c->primeiro->anterior = c->ultimo;
	}
	c->tamanho++;
}


int verifica_cadeia_vazia(cadeiaDNA c)
{
	return (c.primeiro == c.ultimo);
}

void destroi_cadeia(cadeiaDNA *c)
{
	while (!verifica_cadeia_vazia(*c)) {
		c->ultimo = c->ultimo->anterior;
		free (c->ultimo->proximo);
	}
}

void imprime_cadeia_DNA(cadeiaDNA c)
{
	if (verifica_cadeia_vazia(c)) {
		printf("Cadeia vazia!\n");
		return;
	}

	ponteiroNoCadeia aux = c.primeiro->proximo; // pulando célula cabeça
	
	while (aux != NULL && aux != c.primeiro) {
		printf ("%c", aux->item.gene);
		aux = aux->proximo;
	}
}

void imprime_cadeia_DNA_no_arquivo(cadeiaDNA c, FILE *arquivo)
{
	if (verifica_cadeia_vazia(c)) {
		printf("Cadeia vazia!\n");
		return;
	}

	ponteiroNoCadeia aux = c.primeiro->proximo; // pulando célula cabeça

	while (aux != NULL && aux != c.primeiro) {
		fputc ((aux->item.gene), arquivo);
		aux = aux->proximo;
	}
}

ponteiroNoCadeia procura_ponta_cega(cadeiaDNA c)
{
	if (verifica_cadeia_vazia(c)) {
		printf("Cadeia vazia!\n");
		return NULL;
	}

	ponteiroNoCadeia aux = c.primeiro->proximo;

	while (aux != NULL && aux != c.primeiro) {
		if (aux->item.tipo == CEGA)
			return aux;

		aux = aux->proximo;
	}
	return NULL;
}

void copia_cadeia_DNA_ate_ponta_cega(cadeiaDNA *novaCadeia, cadeiaDNA c, ponteiroNoCadeia pontaCega)
{
	ponteiroNoCadeia aux = c.primeiro;
	cria_cadeia_DNA(novaCadeia, LINEAR);

	while (aux != pontaCega) {
		insere_no_cadeia_DNA(aux->item, novaCadeia);
		aux = aux->proximo;
	}
	insere_no_cadeia_DNA(aux->item, novaCadeia); //insere a ponta cega tambem
}

/*LISTA*/
void cria_lista(lista *l)
{
	l->primeiro = (ponteiroNoLista) malloc(sizeof(noLista));
	l->ultimo = l->primeiro;
	l->primeiro->anterior = NULL;
	l->primeiro->proximo = NULL;
}

void insere_no_lista(cadeiaDNA c, lista *l)
{
	ponteiroNoLista aux = l->ultimo;

	l->ultimo-> proximo = (ponteiroNoLista) malloc(sizeof(noLista));
	l->ultimo = l->ultimo->proximo;
	l->ultimo->cadeia = c;
	l->ultimo->anterior = aux;
	l->ultimo->proximo = NULL;
}

int verifica_lista_vazia(lista l)
{
	return (l.primeiro == l.ultimo);
}

void exclue_no_lista(ponteiroNoLista p, lista *l)
{
	if (verifica_lista_vazia(*l)) {
		printf("lista vazia\n");
		return;
	}

	p->anterior->proximo = p->proximo;
	
	if (p == l->ultimo)
		l->ultimo = p->anterior;
	else
		p->proximo->anterior = p->anterior;

	free(p);
}

void imprime_lista(lista l)
{
	ponteiroNoLista aux = l.primeiro->proximo;

	while (aux != NULL) {
		imprime_cadeia_DNA(aux->cadeia);
		printf(" ");
		aux = aux->proximo;
	}
}

void imprime_lista_no_arquivo(lista l, FILE *arquivo)
{
	ponteiroNoLista aux = l.primeiro->proximo;

	while (aux != NULL) {
		imprime_cadeia_DNA_no_arquivo(aux->cadeia, arquivo);
		fputc (' ', arquivo);
		aux = aux->proximo;
	}
}