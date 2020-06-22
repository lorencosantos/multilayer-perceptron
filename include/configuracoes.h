#ifndef INCLUDE_CONFIGURACOES_H_
#define INCLUDE_CONFIGURACOES_H_

typedef struct {
    int num_camadas_ocultas;
    int *neu_por_camada;
    int *funcs_ativ;
    double taxa_aprendizado;
    double tolerancia;
    int max_epocas;
    int neu_camada_saida;
    int func_ativ_saida;
    double **dados_treinamento;
    double **dados_teste;
    int num_features;
    int num_amostras_treinamento;
    int num_amostras_teste;
    double ***pesos;
} config_rede;

#endif /* INCLUDE_CONFIGURACOES_H_ */
