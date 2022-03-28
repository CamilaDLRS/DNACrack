#ifndef __IDENTIFICAOAMINOACIDO__
#define __IDENTIFICAOAMINOACIDO__

/// <summary>
/// ============================================================================
/// Author:         Camila Delarosa
/// Create date:    29/09/2018
/// Description:	Esta biblioteca possui as estruturas de dados especificas e
///					suas respectivas funções de controle para a identificação de
///					aminoácidos. Desenvolvida para o programa DNAcrack.
/// Error control:  Mensagens de erro são registradas na biblioteca Verificacao.
/// ============================================================================
/// </summary>

typedef struct codAminoacido_t
{
	char codificacao[4];
	char aminoacido;
} codAminoacido_t;

/*---------------------------------FUNÇÕES------------------------------------*/
void analyze(char *gene, char *proteina);

int verifica_tamanho_proteina_gene(int proteina, int gene, boolean_t menssagem);

/*TABELA*/
void cria_tabela_codificacoes(codAminoacido_t *tabela);

void limpa_tabela_codificacoes(codAminoacido_t *tabela);

void imprime_tabela_codificacoes(codAminoacido_t tabela[]);

boolean_t procurar_codificacao_tabela(codAminoacido_t codAminoacido, codAminoacido_t tabela[]);

#endif
