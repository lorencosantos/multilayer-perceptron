//
// Created by Xx220xX on 24/06/2020.
//
#include "parametros_io.h"
#include <string.h>
#define CABECALHO "NEURAL NETWORK\n"\
                  "LENGTH OF ARCHITECTURE = 4 BYTES\n"\
                  "ARQUITETURE = 4 * LENGTH_ARCHITECTURE BYTES\n"\
                  "FUNCTION ID = LENGTH_ARCHITECTURE - 1 BYTES\n"\
                  "HITE LEARN = 8 BYTES\n"\
                  "WEIGHTS WITH DIMENSION DEFINED BY ARCHITECTURE\n"

/**
 * Salva em binario os dados da rede
 * os primeiros 210 BYTES sao de cabecalho
 * @param d
 * @param tam_camada tamanho de cada camada
 * @param file_name
 * @return
 */
int salvar_parametros(config_rede * d, int *tam_camada,char *file_name) {
    if (!d)return -2;
    int length = d->num_camadas_ocultas + 2;
    int i, j, l;
    FILE *f = fopen(file_name, "wb");
    if (!f)return -1;

    fwrite(CABECALHO, sizeof(char), 210, f);
    fwrite(&length, sizeof(int), 1, f);
    fwrite(tam_camada, sizeof(int),length,f);
    fwrite(d->funcs_ativ, length - 2, sizeof(int), f);
    fwrite(&d->func_ativ_saida, 1, sizeof(int), f);
    fwrite(&d->taxa_aprendizado, 1, sizeof(double), f);

    double ** w;
    int m,n;
    for (l = 0; l < length-1; l++) {
        w = d->pesos[l];
        // salvar a matriz tranposta
        // m = tam_camada[l]+1 e n = tam_camada[l+1]
        m = tam_camada[l]+1;
        n = tam_camada[l];
        // transpondo
        for (i = 0; i < n; ++i) {
            for (j = 0; i < m; ++i) {
                fwrite(w[j]+i, 1, sizeof(double), f);
            }
        }
    }
    fclose(f);
    return 0;
}

/**
 * Le as configurações de uma rede salva em um arquivo
 *
 * @param tam_camada vetor alocado dentro da função com o tamanho de cada camada
 * @param file_name arquivo para ler os dados
 * @return caso aconteça alguma falha o retorno é nulo, caso contrario um ponteiro para
 *          confi_rede que deve ser liberado com a função free
 */
config_rede * ler_parametros(int **tam_camada,const char *file_name){
    config_rede *d = calloc(sizeof(config_rede),1);
    if (!d)return NULL;
    int length=0;
    int i, j, l;
    FILE *f = fopen(file_name, "wb");
    if (!f) {
        free(d);
        return NULL;
    }
    // verificar cabeçalho
    char cabecalho[211] = {0};
    fread(cabecalho, sizeof(char), 210, f);
    if (strcmp(cabecalho, CABECALHO)) {free(d);return NULL;}

    // ler arquitetura
    fread(&length, sizeof(int), 1, f);
    *tam_camada = calloc(length, sizeof(int));
    if(!*tam_camada){free(d);fclose(f);return NULL;}
    fread(*tam_camada, sizeof(int),length,f);

    // usar arquitetura para separar neuronios da camada oculta
    d->neu_por_camada = calloc(length, sizeof(int));
    if(!d->neu_por_camada){free(*tam_camada);free(d);fclose(f);return NULL;}
    memcpy(d->neu_por_camada,*tam_camada + 1,length-2* sizeof(int));
    // definir numero de features
    d->num_features = (*tam_camada)[0] +1;// main.c linha 229

    // ler funcoes de ativacao
    d->funcs_ativ = calloc(length, sizeof(int));
    if(!d->funcs_ativ){free(*tam_camada);free(d);fclose(f);return NULL;}
    fread(d->funcs_ativ, length - 2, sizeof(int), f);
    fread(&d->func_ativ_saida, 1, sizeof(int), f);

    //ler taxa de aprendizado
    fread(&d->taxa_aprendizado, 1, sizeof(double), f);

    // ler pesos
    //alocas matriz tridimensional
    d->pesos = (double ***)calloc(sizeof(double **),length-1);
    double ** w;
    int m,n;
    for (l = 0; l < length-1; l++) {
        m = *tam_camada[l]+1;
        n = *tam_camada[l];
        // alocar recursos para matriz pesos [l]
        d->pesos[l] = (double **)calloc(sizeof(double *),m);
        for(i=0;i<m;i++){d->pesos[l][i] = (double *)calloc(sizeof(double),n);}
        w = d->pesos[l];
        // ler a matriz tranposta
        // m = tam_camada[l]+1 e n = tam_camada[l+1]
        // transpondo
        for (i = 0; i < n; ++i) {
            for (j = 0; i < m; ++i) {
                fread(w[j]+i, 1, sizeof(double), f);
            }
        }
    }
    fclose(f);
    return d;
}