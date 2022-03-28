#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dados.h"
#include "IdentificacaoAminoacido.h"
#include "Verificacao.h"

int verifica_tamanho_proteina_gene(int proteina, int gene, boolean_t menssagem)
{
	if ((proteina * 3) > gene) {
		if (menssagem == VERDADEIRO)			
			imprime_menssagem(MSG5);
		return 1;
	}
	return 0;
}

/*TABELA*/
void cria_tabela_codificacoes(codAminoacido_t *tabela)
{
	FILE *ponteiro;
	int i = 0;
	ponteiro = fopen("codificacoesPossiveis.txt", "r");

	while (fscanf(ponteiro, "%s", (tabela + i) -> codificacao) != EOF)
		i++;

	fclose(ponteiro);
}

void limpa_tabela_codificacoes(codAminoacido_t *tabela)
{
	for (int i = 0; i < 64; i++)
		(tabela + i) -> aminoacido = '.';
}

void imprime_tabela_codificacoes(codAminoacido_t tabela[])
{
	for (int i = 0; i < 64; i+=4) {
		for (int j = 0; j <= 3; j++)
			printf ("\t%s %c", tabela[i+j].codificacao, tabela[i+j].aminoacido);
		printf ("\n");
	}
}

boolean_t procura_codificacao_tabela(codAminoacido_t codAminoacido, codAminoacido_t tabela[])
{
	int i = 0;
	while (strcmp(codAminoacido.codificacao, tabela[i].codificacao))
		i++;

	if (tabela[i].aminoacido == '.' || tabela[i].aminoacido == codAminoacido.aminoacido) {
		tabela[i].aminoacido = codAminoacido.aminoacido;
		return VERDADEIRO;
	}
	return FALSO;
}


void analyze(char *gene, char *proteina)
{
	FILE *geneArq;
	FILE *proteinaArq;
	int tamanhoGene = 0, tamanhoProteina = 0;

/*---------------------------PROTEINA-VERIFICAÇÃO-----------------------------*/
	proteinaArq = fopen (proteina, "r");
	tamanhoProteina = verifica_arquivo_descricao_proteina(proteinaArq);
	rewind(proteinaArq);

/*-----------------------PREENCHIMENTO-E-VERIFICAÇÃO--------------------------*/

/*CADEIA*/
	cadeiaDNA seqGenetica;
	cria_cadeia_DNA(&seqGenetica, LINEAR);

	geneArq = fopen (gene, "r");

	itemCadeia_t item;
	item.gene = fgetc (geneArq);
	item.tipo = NORMAL;

	do {
		verifica_elemento_arquivo_sequencia_genetica(item.gene);
		insere_no_cadeia_DNA(item, &seqGenetica);
		tamanhoGene++;
		item.gene = fgetc (geneArq);
	} while (item.gene != EOF);

	fclose (geneArq);

/*PROTEINA*/
	char descProteina[tamanhoProteina];
	char aminoacido = fgetc (proteinaArq);
	int posicao = 0;

	do {
		descProteina[posicao] = aminoacido;
		posicao++;
		aminoacido = fgetc (proteinaArq);
	} while (aminoacido != EOF);

	fclose (proteinaArq);

/*---------------------------------TAMANHO------------------------------------*/
	if (verifica_tamanho_proteina_gene(tamanhoProteina, tamanhoGene, VERDADEIRO))
		exit(1);

/*-------------------------CODIFICACOES-POSSIVEIS-----------------------------*/
	codAminoacido_t tabela[65];
	cria_tabela_codificacoes(tabela);
	limpa_tabela_codificacoes(tabela);

/*--------------------------------CASAMENTO-----------------------------------*/
	boolean_t casamento = VERDADEIRO;
	ponteiroNoCadeia andadorSeq = seqGenetica.primeiro -> proximo; //pulando celula cabeça
	ponteiroNoCadeia aux = andadorSeq;
	//preechemos essa variavel com uma codificação lida do gene e o aminoacido lido da proteina que queremos atribuir a ela
	codAminoacido_t codEAminoacido;
	int contProteina = 0;
	int cpTamanhoGene = tamanhoGene;

	do {
		for (int i = 0; i < 3; i++) {
			codEAminoacido.codificacao[i] = andadorSeq -> item.gene;
			andadorSeq = andadorSeq -> proximo;
		}
		codEAminoacido.codificacao[3] = '\0';
		codEAminoacido.aminoacido = descProteina[contProteina];

		casamento = procura_codificacao_tabela(codEAminoacido, tabela);

		if (casamento == VERDADEIRO)
			contProteina++;
		else {
			limpa_tabela_codificacoes(tabela);
			andadorSeq = aux -> proximo; // proxima verificacao
			aux = andadorSeq;
			contProteina = 0;
			cpTamanhoGene--;
		}
	} while (!verifica_tamanho_proteina_gene(tamanhoProteina, cpTamanhoGene, FALSO) && contProteina < tamanhoProteina);

/*----------------------------------SAIDA-------------------------------------*/
	if (casamento == FALSO)
		printf ("\tCasamento nao encontrado.\n");
	else {
		printf("\tCasamento encontrado na posicao %d\n", tamanhoGene - cpTamanhoGene);
		imprime_tabela_codificacoes(tabela);
	}
}
