#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Verificacao.h"

void imprime_menssagem(menssagem_t msg)
{
	switch (msg)
	{
		case MSG1:
			printf("ERRO: Parametros invalidos. Verifique as possiveis entradas para as seguintes funcionalidades:\n\n"
						"	Identificacao de aminoacidos: \n\t./DNAcrack -analyze 'nome-arquivo-gene'.txt 'nome-arquivo-proteina'.txt\n\n"
						"	Recortar DNA linear: \n\t./DNAcrack -cut -l 'nome-arquivo-dna'.txt 'nome-arquivo-enzima'.txt\n\n"
						"	Recombinar DNA linear: \n\t./DNAcrack -glue -l 'nome-arquivo-dna-1'.txt 'nome-arquivo-dna-2'.txt\n\n"
						"	Recombinar DNA linear com enzima: \n\t./DNAcrack -recombine -l 'nome-arquivo-dna-1'.txt 'nome-arquivo-dna-2'.txt 'nome-arquivo-enzima'.txt\n\n"
						"	Recombinar DNA circular com enzima: \n\t./DNAcrack -recombine -c 'nome-arquivo-dna-1'.txt 'nome-arquivo-dna-2'.txt 'nome-arquivo-enzima'.txt\n");
		break;

		case MSG2:
			printf("ERRO: Execucao do(s) arquivo(s). Possiveis motivos:\n"
						"	A - Inexistente\n"
						"	B - Nao possue permissao para le-lo\n"
						"	C - Esta bloqueado (sendo usado por outro programa)\n");
		break;

		case MSG3:
			printf("INVALIDO: arquivos que contem sequencia genetica nao podem possuir caracter(s) diferente(s) de 'A', 'C', 'G' ou 'T'.\n");
		break;

		case MSG4:
			printf("INVALIDO: arquivos que contem descricao de proteina nao podem possuir caractere(s) que fogem do limite [a..y] da tabela ASCII.\n");
		break;

		case MSG5:
			printf("INVALIDO: A sequencia genetica precisa ser no minimo 3 vezes maior que a proteina.\n");
		break;

		case MSG6:
			printf("INVALIDO: O recorte de cadeias ciruculares nao foi implementado.\n");
		break;

		case MSG7:
			printf("INVALIDO: A colagem de cadeias circulares nao foi implementada.\n");
		break;

		case MSG8:
			printf("INVALIDO: a enzima nao pode ser maior que o DNA.\n");
		break;

		case MSG9:
			printf("INVALIDO: Arquivo(s) vazio(s).\n");
		break;
	}
}

void verifica_arquivos(int quantidade, char *argv[])
{
	FILE *ponteiroArqUm, *ponteiroArqDois;

	if (quantidade == 4) {

		ponteiroArqUm = fopen(argv[2], "r");
		ponteiroArqDois = fopen(argv[3], "r");

		if ((ponteiroArqUm == NULL) || (ponteiroArqDois == NULL)) {
			imprime_menssagem(MSG2);
			exit(1);
		}

		/*VERIFICA SE ESTA VAZIO*/
		fseek (ponteiroArqUm, 0, SEEK_END);
		fseek (ponteiroArqDois, 0, SEEK_END);
		if ((ftell (ponteiroArqUm) == 0) || (ftell (ponteiroArqDois) == 0)) {
			imprime_menssagem(MSG9);
			exit(1);
		}
	}
	else if (quantidade >= 5) {

		ponteiroArqUm = fopen(argv[3], "r");
		ponteiroArqDois = fopen(argv[4], "r");

		if ((ponteiroArqUm == NULL) || (ponteiroArqDois == NULL)) {
			imprime_menssagem(MSG2);
			exit(1);
		}

		fseek (ponteiroArqUm, 0, SEEK_END);
		fseek (ponteiroArqDois, 0, SEEK_END);
		if ((ftell (ponteiroArqUm) == 0) || (ftell (ponteiroArqDois) == 0)) {
			imprime_menssagem(MSG9);
			exit(1);
		}		
	}
	if (quantidade == 6) {

		FILE * ponteiroArqTres;
		ponteiroArqTres = fopen(argv[5], "r");

		if (ponteiroArqTres == NULL) {
			imprime_menssagem(MSG2);
			exit(1);
		}

		fseek (ponteiroArqTres, 0, SEEK_END);
		if (ftell (ponteiroArqTres) == 0) {
			imprime_menssagem(MSG9);
			exit(1);
		}

		if (ponteiroArqTres == NULL) {
			imprime_menssagem(MSG2);
			exit(1);
		}

		fclose (ponteiroArqTres);
	}
	fclose (ponteiroArqUm);
	fclose (ponteiroArqDois);
}

entrada_t verifica_entradas(int argc, char *argv[])
{
	if (argc == 4) {

		if (!strcmp(argv[1], "-analyze")) {
			verifica_arquivos(4, argv);
			return ANALYZE;
		}
	}
	else if (argc == 5) {

		if (!strcmp(argv[1], "-cut")) {

			if (!strcmp(argv[2], "-l")) {
				verifica_arquivos(5, argv);
				return CUT;
			}
			if (!strcmp(argv[2], "-c")) {
				imprime_menssagem(MSG6);
				return ERRO;
			}
		}
		else if (!strcmp(argv[1], "-glue")) {

			if (!strcmp(argv[2], "-l")) {
				verifica_arquivos(5, argv);
				return GLUE;
			}
			if (!strcmp(argv[2], "-c")) {
				imprime_menssagem(MSG7);
				return ERRO;
			}
		}
	}
	else if (argc == 6) {

		FILE *ponteiroArqTres;

		if (!strcmp(argv[1], "-recombine")) {

			if (!strcmp(argv[2], "-l")) {
				verifica_arquivos(6, argv);					
				return RECOMBINE_L;
			}
			if (!strcmp(argv[2], "-c")) {
				verifica_arquivos(6, argv);
				return RECOMBINE_C;
			}
		}
	}
	
	imprime_menssagem(MSG1);
	return ERRO;
}

void verifica_elemento_arquivo_sequencia_genetica(char gene)
{
	if (!((gene == 'A') || (gene == 'C') || (gene == 'G') || (gene == 'T') || (gene == 'a') || (gene == 'c') || (gene == 'g') || (gene == 't'))) {
		imprime_menssagem(MSG3);
		exit(1);
	}
}

int verifica_arquivo_descricao_proteina(FILE *ponteiroArq)
{
	char leitor = fgetc (ponteiroArq);
	int tamanhoProteina = 0;
	do {
		tamanhoProteina++;

		if (leitor < 'a' || leitor > 'y') {
			imprime_menssagem(MSG4);
			exit(1);
		}
		leitor = fgetc (ponteiroArq);
	}while (leitor != EOF);

	return tamanhoProteina;
}
