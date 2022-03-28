#include <stdio.h>
#include <string.h>
#include "Verificacao.h"
#include "Dados.h"
#include "IdentificacaoAminoacido.h"
#include "Recorte.h"
#include "Colagem.h"
#include "Recombinacao.h"

int main(int argc, char **argv)
{
	/// <summary>
    /// ===========================================================================
    /// Author:         Camila Delarosa
    /// Create date:    29/09/2018
    /// Description:    Este programa possui como funcionalidades a identificação
    ///					de aminoácios, simulação de recorte, colagem e recombinação
	///					de DNA por enzima de restrição.
    /// Error control:  Mensagens de erro são registradas na biblioteca Verificacao.
	/// Observation:	*As funcionalidade de recombinação e colagem de DNA estão
	///					em desenvolvimento*.
    /// ===========================================================================
    /// </summary>
	entrada_t entrada = verifica_entradas(argc, argv);

	if (entrada == ANALYZE)
		analyze(argv[2], argv[3]);

	else if (entrada == CUT)
		cut(argv[3], argv[4], LINEAR, NULL, FALSO, NULL);

	else if (entrada == GLUE)
		glue(argv[3], argv[4]);

	else if (entrada == RECOMBINE_L)
		recombine_l(argv[3], argv[4], argv[5]);

	else if (entrada == RECOMBINE_C)
		recombine_c(argv[3], argv[4], argv[5]);

	return 0;
}
