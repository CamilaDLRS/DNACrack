#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dados.h"
#include "Recorte.h"
#include "Verificacao.h"

int verifica_pontas_cegas(cadeiaDNA c1, cadeiaDNA c2)
{
	ponteiroNoCadeia auxC1 = c1.ultimo, auxC2 = c2.ultimo;

	if (auxC1->item.gene == auxC2->item.gene)
		return 1;

	return 0;
}

int verifica_pontas_adesivas(cadeiaDNA c1, cadeiaDNA c2)
{
	ponteiroNoCadeia auxC1 = c1.primeiro->proximo, auxC2 = c2.primeiro->proximo;
	
	while (auxC1->item.tipo == ADESIVA && auxC2->item.tipo == ADESIVA) {
		if (auxC1->item.gene != auxC2->item.gene)
			break;
		auxC1 = auxC1->proximo;
		auxC2 = auxC2->proximo;
	}
	if (auxC1->item.tipo == NORMAL && auxC2->item.tipo == NORMAL)
		return 1;

	return 0;
}

void cola_tres_cadeias(ponteiroNoLista c1, ponteiroNoLista c2, ponteiroNoLista c3, lista *l1)
{
	c1->cadeia.ultimo->proximo = c2->cadeia.primeiro->proximo;
	c2->cadeia.primeiro->proximo->anterior = c1->cadeia.ultimo;

	c2->cadeia.ultimo->proximo = c3->cadeia.primeiro->proximo;
	c3->cadeia.primeiro->proximo->anterior = c2->cadeia.ultimo;
	c1->cadeia.ultimo = c3->cadeia.ultimo;

	c1->cadeia.tamanho += c2->cadeia.tamanho + c3->cadeia.tamanho;

	c3->anterior->proximo = c3->proximo;
	if (c3->proximo != NULL)
		c3->proximo->anterior = c3->anterior;
	else
		c3->anterior->proximo = l1->ultimo;

	c2->anterior->proximo = c2->proximo;
	if (c2->proximo != NULL)
		c2->proximo->anterior = c2->anterior;
}

void cola_circular(ponteiroNoLista c1, ponteiroNoLista c2)
{
	c1->cadeia.ultimo->proximo = c2->cadeia.primeiro->proximo;
	c2->cadeia.primeiro->proximo->anterior = c1->cadeia.ultimo;
	c2->cadeia.ultimo->proximo = c1->cadeia.primeiro;
	c1->cadeia.primeiro->anterior = c2->cadeia.ultimo;
	c1->cadeia.ultimo = c2->cadeia.ultimo;

	c1->cadeia.tamanho += c2->cadeia.tamanho;

	c2->anterior->proximo = c2->proximo;
	if (c2->proximo != NULL)
		c2->proximo->anterior = c2->anterior;

	
	printf("Uma nova cadeia circular foi criada: ");
	imprime_cadeia_DNA(c1->cadeia);
	printf("\n");
	c1->cadeia.tipo = CIRCULAR;
}

void glue(char *dna1, char *dna2)
{
	FILE *dna1Arq;
	FILE *dna2Arq;
	FILE *outputArq;
	FILE *dumpArq;

/*--------------------------PREENCHIMENTO-E-VERIFICAÇÃO-------------------------*/
	/*DNA 1*/
	lista listaDna1;
	cria_lista(&listaDna1);
	cadeiaDNA copiaSeq;
	cria_cadeia_DNA(&copiaSeq, LINEAR);
	dna1Arq = fopen (dna1, "r");
	itemCadeia_t item;

	do {			
		item.tipo = ADESIVA;
		item.gene = fgetc (dna1Arq);
		do {
			verifica_elemento_arquivo_sequencia_genetica(item.gene);

			if (item.gene < 97 && item.tipo == ADESIVA)
				item.tipo = NORMAL;
			else if (item.gene >= 97 && item.tipo == NORMAL)
				item.tipo = CEGA;

			insere_no_cadeia_DNA(item, &copiaSeq);
			item.gene = fgetc (dna1Arq);
		} while (item.gene != ' ' && item.gene != EOF);

		insere_no_lista(copiaSeq, &listaDna1);
		cria_cadeia_DNA(&copiaSeq, LINEAR);
		//tentar excluir a cadeia inteira, pra ver se ela ainda existira dentro da lista
	} while (item.gene != EOF);

	fclose (dna1Arq);

	/*DNA 2*/
	lista listaDna2;
	cria_lista(&listaDna2);
	dna2Arq = fopen (dna2, "r");

	do {			
		item.tipo = ADESIVA;
		item.gene = fgetc (dna2Arq);
		do {
			verifica_elemento_arquivo_sequencia_genetica(item.gene);

			if (item.gene < 97 && item.tipo == ADESIVA)
				item.tipo = NORMAL;
			else if (item.gene >= 97 && item.tipo == NORMAL)
				item.tipo = CEGA;

			insere_no_cadeia_DNA(item, &copiaSeq);
			item.gene = fgetc (dna2Arq);
		} while (item.gene != ' ' && item.gene != EOF);

		insere_no_lista(copiaSeq, &listaDna2);
		cria_cadeia_DNA(&copiaSeq, LINEAR);
	} while (item.gene != EOF);

	fclose (dna2Arq);

/*-------------------------------COLAGEM-PAR-A-PAR------------------------------*/

	ponteiroNoLista auxDna2 = listaDna2.primeiro->proximo;
	ponteiroNoLista auxDna1 = listaDna1.primeiro->proximo, auxParDna1 = auxDna1->proximo;
	boolean_t colou;

	while (auxDna2 != NULL && auxParDna1 != NULL) {

		colou = FALSO;
		while (auxParDna1 != NULL) {

			if (verifica_pontas_cegas(auxDna1->cadeia, auxDna2->cadeia) == 1 ||
					verifica_pontas_adesivas(auxDna2->cadeia, auxDna1->cadeia) == 1) {

				while (auxParDna1 != NULL) {

					if (verifica_pontas_cegas(auxDna1->cadeia, auxDna2->cadeia) == 1) {
						if (verifica_pontas_adesivas(auxDna2->cadeia, auxParDna1->cadeia) == 1) {
	//						auxDna2 = auxDna2->anterior;
	//						cola_tres_cadeias(auxDna1, auxDna2->proximo, auxParDna1);
							cola_tres_cadeias(auxDna1, auxDna2, auxParDna1, &listaDna1);
							auxParDna1 = NULL;
							colou = VERDADEIRO;
							break;
						}
					}
					if (verifica_pontas_cegas(auxParDna1->cadeia, auxDna2->cadeia) == 1) {
						if (verifica_pontas_adesivas(auxDna2->cadeia, auxDna1->cadeia) == 1) {
							//auxDna2 = auxDna2->anterior;
							cola_tres_cadeias(auxParDna1, auxDna2, auxDna1, &listaDna1);
							auxParDna1 = NULL;
							colou = VERDADEIRO;
							break;
						}
					}

					auxParDna1 = auxParDna1->proximo;
				}
			}
			else
				auxParDna1 = NULL;

			if (auxParDna1 == NULL && colou == FALSO) {
				auxDna1 = auxDna1->proximo;
				auxParDna1 = auxDna1->proximo;
			}
		}

		auxDna2 = auxDna2->proximo;
		auxDna1 = listaDna1.primeiro->proximo;
		auxParDna1 = auxDna1->proximo;
	};

/*-------------------------------COLAGEM-CIRCULAR-------------------------------*/
	auxDna2 = listaDna2.primeiro->proximo;

	while (auxDna2 != NULL) {
		auxDna1 = listaDna1.primeiro->proximo;

		while (auxDna1 != NULL) {

			if (auxDna1->cadeia.tipo != CIRCULAR)
				if (verifica_pontas_cegas(auxDna1->cadeia, auxDna2->cadeia) == 1)
					if (verifica_pontas_adesivas(auxDna2->cadeia, auxDna1->cadeia) == 1) {
					//	auxDna2 = auxDna2->anterior;
					//	cola_circular(auxDna1, auxDna2->proximo);
						cola_circular(auxDna1, auxDna2);
						break;
					}
			auxDna1 = auxDna1->proximo;
		}
		auxDna2 = auxDna2->proximo;
	}

/*-------------------------------------SAIDA------------------------------------*/
	/*identifica maior cadeia*/
	ponteiroNoLista aux = listaDna1.primeiro->proximo;
	ponteiroNoLista maior = aux;
	aux = aux->proximo;

	while (aux != NULL) {
		if (aux->cadeia.tamanho > maior->cadeia.tamanho)
			maior = aux;
		aux = aux->proximo;
	}

	outputArq = fopen ("output.txt", "w");
	imprime_cadeia_DNA_no_arquivo(maior->cadeia, outputArq);
	fclose (outputArq);
	printf("\n Arquivo criado com sucesso!(output.txt)\n Verifique no diretorio do programa.\n");

	exclue_no_lista(maior, &listaDna1);

	dumpArq = fopen ("dump.txt", "w");
	imprime_lista_no_arquivo(listaDna1, dumpArq);
	fprintf (dumpArq, " "); //espaço extra para  identificação
	imprime_lista_no_arquivo(listaDna2, dumpArq);
	fclose (dumpArq);
	printf("\n Arquivo criado com sucesso!(dump.txt)\n Verifique no diretorio do programa.\n");
}
