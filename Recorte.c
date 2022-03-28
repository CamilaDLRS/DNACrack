#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dados.h"
#include "Recorte.h"
#include "Verificacao.h"

//destroi a enzima e transfere a sequencia do DNA criada para a saida
void retorna_recombine(ponteiroCadeia saida, cadeiaDNA seqDNA, cadeiaDNA enzima)
{
	saida->primeiro = seqDNA.primeiro;
	saida->ultimo = seqDNA.ultimo;
	saida->tamanho = seqDNA.tamanho;
	destroi_cadeia(&enzima);
}

void cut(char *dna, char *enzima, tipoCadeia_t tipoCadeia, ponteiroCadeia ponteiroCadeiaSaida, boolean_t recombine, boolean_t *ocorrenciaDeCorte)
{
	FILE *dnaArq;
	FILE *enzimaArq;
	FILE *saidaArq;

/*------------------------PREENCHIMENTO-E-VERIFICAÇÃO-------------------------*/
	/*ENZIMA*/
	cadeiaDNA seqGeneticaEnzima;
	cria_cadeia_DNA(&seqGeneticaEnzima, LINEAR);

	enzimaArq = fopen (enzima, "r");

	itemCadeia_t item;
	item.gene = fgetc (enzimaArq);
	item.tipo = NORMAL;

	do {
		verifica_elemento_arquivo_sequencia_genetica(item.gene);
		insere_no_cadeia_DNA(item, &seqGeneticaEnzima);
		item.gene = fgetc (enzimaArq);
	} while (item.gene != EOF);

	fclose (enzimaArq);

	/*DNA*/
	cadeiaDNA seqGeneticaDNA;
	cria_cadeia_DNA(&seqGeneticaDNA, tipoCadeia);

	dnaArq = fopen (dna, "r");
	item.gene = fgetc (dnaArq);
	item.tipo = NORMAL;

	do {
		verifica_elemento_arquivo_sequencia_genetica(item.gene);
		if (recombine == VERDADEIRO && ((item.gene == 'a') || (item.gene == 'c') || (item.gene == 't') || (item.gene == 'g')))
			item.tipo = ADESIVA;
		
		insere_no_cadeia_DNA(item, &seqGeneticaDNA);
		item.gene = fgetc (dnaArq);
	} while (item.gene != EOF);

	fclose (dnaArq);

	/*TAMANHO*/
	if (seqGeneticaEnzima.tamanho > seqGeneticaDNA.tamanho) {
		imprime_menssagem(MSG8);
		exit(1);
	}

/*------------------------ETIQUETAÇÃO-DOS-AMINOACIDOS-------------------------*/
	ponteiroNoCadeia andadorDNA = seqGeneticaDNA.primeiro -> proximo;
	ponteiroNoCadeia andadorEnzima;
	ponteiroNoCadeia transferidorDNA; // auxiliar para a etiquetação
	int cpTamanhoDNA = seqGeneticaDNA.tamanho;
	int cpTamanhoEnzima;
	int cortes = 0;

	if (tipoCadeia == CIRCULAR)
		cpTamanhoDNA += cpTamanhoEnzima - 1;

	do {
		andadorEnzima = seqGeneticaEnzima.primeiro -> proximo; //pulando célula cabeça
		cpTamanhoEnzima = seqGeneticaEnzima.tamanho;

		while (andadorEnzima != NULL && cpTamanhoEnzima <= cpTamanhoDNA) {
			if ((andadorDNA -> item.gene) == (andadorEnzima -> item.gene)) {
				andadorDNA = andadorDNA -> proximo;
				cpTamanhoDNA--;
				if (andadorDNA == seqGeneticaDNA.primeiro) //caso seja circular
					andadorDNA = andadorDNA -> proximo;

				andadorEnzima = andadorEnzima -> proximo;
				cpTamanhoEnzima--;
			}
			else if (andadorEnzima != seqGeneticaEnzima.primeiro -> proximo) { //ja andamos na enzima?
				andadorEnzima = seqGeneticaEnzima.primeiro -> proximo; //reinicia enzima
				cpTamanhoEnzima = seqGeneticaEnzima.tamanho;
			}
			else {
				andadorDNA = andadorDNA -> proximo;
				cpTamanhoDNA--;
				if (andadorDNA == seqGeneticaDNA.primeiro) //caso seja circular
					andadorDNA = andadorDNA -> proximo;
			}
		}

		/*ETIQUETANDO...*/
		if (cpTamanhoEnzima == 0) {
			
			cortes++;

			//caso a um enzima no final do DNA, o andador vai para o final
			if (cpTamanhoDNA == 0) {
				andadorDNA = seqGeneticaDNA.ultimo;
				transferidorDNA = andadorDNA;
			}
			else {
				transferidorDNA = andadorDNA -> anterior;
				if(transferidorDNA == seqGeneticaDNA.primeiro)
					transferidorDNA = transferidorDNA -> anterior;
			}

			for (int j = 1; j < seqGeneticaEnzima.tamanho; j++) {
				transferidorDNA -> item.tipo = ADESIVA;
				transferidorDNA -> item.gene += 32;
				transferidorDNA = transferidorDNA -> anterior;
				if(transferidorDNA == seqGeneticaDNA.primeiro)
					transferidorDNA = transferidorDNA -> anterior;
			}
			transferidorDNA -> item.tipo = CEGA;
			transferidorDNA -> item.gene += 32;

			*ocorrenciaDeCorte = VERDADEIRO;
		}
		
		//circular no recombine corta apenas uma vez (pode nao ter sido cortado)
		if (recombine == VERDADEIRO && tipoCadeia == CIRCULAR) {
			retorna_recombine(ponteiroCadeiaSaida, seqGeneticaDNA, seqGeneticaEnzima);
			return;
		}

	} while (cpTamanhoEnzima <= cpTamanhoDNA);

	//a sequencia linear do recombine será retornada aqui
	if (recombine == VERDADEIRO) {
		retorna_recombine(ponteiroCadeiaSaida, seqGeneticaDNA, seqGeneticaEnzima);
		return;
	}

/*-----------------------------------LISTA------------------------------------*/
	lista lista;
	cria_lista(&lista);
	cadeiaDNA novaCadeia;

	ponteiroNoCadeia auxPontaCega = procura_ponta_cega(seqGeneticaDNA);
	seqGeneticaDNA.primeiro = seqGeneticaDNA.primeiro -> proximo; // pulando celula cabeça

	while (auxPontaCega != NULL) {

		copia_cadeia_DNA_ate_ponta_cega(&novaCadeia, seqGeneticaDNA, auxPontaCega);
		insere_no_lista(novaCadeia, &lista);

		seqGeneticaDNA.primeiro = auxPontaCega -> proximo;
		auxPontaCega = procura_ponta_cega(seqGeneticaDNA);
	}

	//cria um nova célula cabeça, pois esta foi perdida com as quebras da cadeia ou simplesmente pulada
	seqGeneticaDNA.primeiro = seqGeneticaDNA.primeiro -> anterior;
	insere_no_lista(seqGeneticaDNA, &lista);

/*-----------------------------------SAIDA------------------------------------*/
	saidaArq = fopen ("recorte-da-cadeia.txt", "w");
	imprime_lista_no_arquivo(lista, saidaArq);
	fclose (saidaArq);
	printf("\n Arquivo criado com sucesso!(recorte-da-cadeia.txt)\n Verifique no diretorio do programa.\n");
}
