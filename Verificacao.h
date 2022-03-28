#ifndef __VERIFICACAO__
#define __VERIFICACAO__

/// <summary>
/// ========================================================================
/// Author:         Camila Delarosa
/// Create date:    29/09/2018
/// Description:	Esta biblioteca é ultilizada no programa DNAcrack (e na 
///					maioria de suas bibliotecas) como recurso para o controle
///					de erros gerais.
/// ========================================================================
/// </summary>

/*-----------------------------TIPOS-DE-DADOS---------------------------------*/
typedef enum entrada_t
{
	ERRO,
	ANALYZE,
	CUT,
	GLUE,
	RECOMBINE_L,
	RECOMBINE_C
} entrada_t;

typedef enum menssagem_t
{
	MSG0,
	MSG1,
	MSG2,
	MSG3,
	MSG4,
	MSG5,
	MSG6,
	MSG7,
	MSG8,
	MSG9
} menssagem_t;

/*---------------------------------FUNÇÕES------------------------------------*/
void imprime_menssagem(menssagem_t msg);

void verificar_arquivos(int quantidade, char *argv[]);

entrada_t verifica_entradas(int argc, char *argv[]);

void verifica_elemento_arquivo_sequencia_genetica(char gene);

int verifica_arquivo_descricao_proteina(FILE *ponteiroArq);

#endif
