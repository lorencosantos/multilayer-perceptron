//
// Created by Xx220xX on 24/06/2020.
//

#ifndef PARAMETROS_IO_H
#define PARAMETROS_IO_H

#include "configuracoes.h"

// configuracao da rede , tamanho das camadas, nome do arquivo
int salvar_parametros(config_rede * , int *,char *);
//tamanho  das camadas, nome do arquivo
config_rede * ler_parametros(int **,const char *);
#endif //PARAMETROS_IO_H
