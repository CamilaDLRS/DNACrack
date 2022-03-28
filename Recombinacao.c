#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dados.h"
#include "Recorte.h"
#include "Recombinacao.h"
#include "Verificacao.h"

void recombine_l(char *dna1, char *dna2, char *enzima) {
	printf("Professor nao quer mais esse.\n");
}

void recombine_c(char *dna1, char *dna2, char *enzima)
{	
	FILE *saidaArq;

	ponteiroCadeia seqCircularDNA = malloc(sizeof(cadeiaDNA));
	seqCircularDNA->tipo = CIRCULAR;
	ponteiroCadeia seqLinearDNA = malloc(sizeof(cadeiaDNA));
	seqCircularDNA->tipo = LINEAR;

	boolean_t circularBool = FALSO, linearBool = FALSO;

	cut(dna2, enzima, LINEAR, seqLinearDNA, VERDADEIRO, &linearBool);

	if (linearBool == VERDADEIRO) {
		cut(dna1, enzima, CIRCULAR, seqCircularDNA, VERDADEIRO, &circularBool);
		
		if (circularBool == VERDADEIRO) {
			ponteiroNoCadeia auxLinear = seqLinearDNA->primeiro->proximo;
			ponteiroNoCadeia auxCircular = seqCircularDNA->primeiro->proximo;

			while (auxCircular->item.tipo != CEGA)
				auxCircular = auxCircular->proximo;

			while (auxLinear->item.tipo != ADESIVA)
				auxLinear = auxLinear->proximo;

			ponteiroNoCadeia auxAdesivaCircular = auxCircular->proximo;
			auxCircular->proximo = auxLinear;

			auxCircular = auxCircular->proximo;
			while (auxCircular->item.tipo != CEGA)
				auxCircular = auxCircular->proximo;

			auxCircular->proximo = auxAdesivaCircular; /*excluir dna2 depois nao esquecer, se tiver tempo*/

			printf("OBS: Colagem feita com êxito!\n");
		}
		else {
			printf("OBS: Não houve mudanças na cadeia circular pois não foi possivel cortar-la.\n");
		}
	}
	else {
		/*Cria cadeia circular caso não precise de mudanças*/
		cria_cadeia_DNA(seqCircularDNA, CIRCULAR);

		FILE * dna1Arq;
		dna1Arq = fopen (dna1, "r");

		itemCadeia_t item;
		item.gene = fgetc (dna1Arq);
		item.tipo = NORMAL;

		do {
			verifica_elemento_arquivo_sequencia_genetica(item.gene);
			insere_no_cadeia_DNA(item, seqCircularDNA);
			item.gene = fgetc (dna1Arq);
		} while (item.gene != EOF);

		fclose (dna1Arq);
		printf("OBS: Não houve mudanças na cadeia circular pois não há segmento disponivel na linear.\n");
	}
/*----------------------------------------SAIDA---------------------------------------*/
	saidaArq = fopen ("dna-circular-resultante.txt", "w");
	imprime_cadeia_DNA_no_arquivo(*seqCircularDNA, saidaArq);
	fclose (saidaArq);
	printf("\n Arquivo criado com sucesso!(dna-circular-resultante.txt)\n Verifique no diretorio do programa.\n");
}