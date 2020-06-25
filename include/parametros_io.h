//
// Created by Xx220xX on 24/06/2020.
//

#ifndef IC_LORE_CONFIG_IO_H
#define IC_LORE_CONFIG_IO_H

#include "../include/configuracoes.h"

// configuracao da rede , tamanho das camadas, nome do arquivo
int salvar_parametros(config_rede * , int *,char *);
//tamanho  das camadas, nome do arquivo
config_rede * ler_parametros(int **,const char *);
#endif //IC_LORE_CONFIG_IO_H
