#ifndef __RECORTE__
#define __RECORTE__

/// <summary>
/// ===========================================================================
/// Author:         Camila Delarosa
/// Create date:    29/09/2018
/// Description:	Esta biblioteca possui uma unica função de controle para o 
///					recorte de DNA por enzimas. Desenvolvida para o programa 
///					DNAcrack.
/// Error control:  Mensagens de erro são registradas na biblioteca Verificacao.
/// ===========================================================================
/// </summary>

void cut(char *dna, char *enzima, tipoCadeia_t tipoCadeia, ponteiroCadeia ponteiroCadeiaSaida, boolean_t recombine, boolean_t *ocorrenciaDeCorte);
#endif
