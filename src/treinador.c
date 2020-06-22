#include "treinador.h"

void inicializar_pesos(config_rede *param, int num_camadas, int *tam_camadas) {
    srand(time(0));

    // epsilon = sqrt(6/(tam_camadas[i] + tam_camadas[i+1])) utilizado para inicializacao
    double *epsilon = (double *)calloc(num_camadas-1, sizeof(double));
    if(epsilon == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    int i;
    for (i = 0; i < num_camadas-1; i++)
        epsilon[i] = sqrt(6.0 / (tam_camadas[i] + tam_camadas[i+1]));

    // inicializacao aleatoria entre [-epsilon[i], epsilon[i]] para o peso[i]
    int j, k;
    for (i = 0; i < num_camadas-1; i++)
        for (j = 0; j < tam_camadas[i]+1; j++)
            for (k = 0; k < tam_camadas[i+1]; k++)
                param->pesos[i][j][k] = -epsilon[i] + ((double)rand() / ((double)RAND_MAX / (2.0 * epsilon[i])));

    // libera memoria alocada
    free(epsilon);
}

void ordenar_aleatoriamente(int *a, int n) {
    int i, j;
    srand(time(NULL));
    for (i = n-1; i > 0; i--) {
        j = rand() % (i+1);
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

void treinador(config_rede *param, int *tam_camadas) {
    // Nro total de camadas
    int num_camadas = param->num_camadas_ocultas + 2;

    // Aloca memoria para os arrays de entradas das camadas
    double **entradas_camadas = (double **)calloc(num_camadas, sizeof(double *));
    if(entradas_camadas == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    int i;
    for (i = 0; i < num_camadas; i++)
    {
        entradas_camadas[i] = (double *)calloc(tam_camadas[i], sizeof(double));
        if(entradas_camadas[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Aloca memoria para os arrays das saidas das camadas
    double **saidas_camadas = (double **)calloc(num_camadas, sizeof(double *));
    if(saidas_camadas == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    for (i = 0; i < num_camadas; i++)
    {
        saidas_camadas[i] = (double*)calloc(tam_camadas[i]+1, sizeof(double));
        if(saidas_camadas[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Inicializa os pesos
    inicializar_pesos(param, num_camadas, tam_camadas);

    // Aloca memoria para os indices das amostras
    int *indices = (int *)calloc(param->num_amostras_treinamento, sizeof(int));
    if(indices == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    for (i = 0; i < param->num_amostras_treinamento; i++)
        indices[i] = i;

    // Treina a rede
    int amostra_atual, j;
    double LMS = 99999;
    double saida_final, erro;
    double delta = 9999999;
    for (i = 0; i < param->max_epocas && delta > param->tolerancia; i++) {
        printf("Epoca %d de %d(max)\r", i+1, param->max_epocas);
        LMS = 0;

        // ordena os indices aleatoriamente
        ordenar_aleatoriamente(indices, param->num_amostras_treinamento);

        for (j = 0; j < param->num_amostras_treinamento; j++) {
            amostra_atual = indices[j];
            // realiza a propagacao direta na j-esima amostra
            forward_propagation(param, amostra_atual, num_camadas, tam_camadas, entradas_camadas, saidas_camadas);

            // Calculo do erro
            saida_final = saidas_camadas[num_camadas-1][1];
            erro = saida_final - param->dados_treinamento[amostra_atual][param->num_features-1];
            LMS += erro * erro;

            // realiza a retro propagacao e atualiza os pesos
            back_propagation(param, amostra_atual, num_camadas, tam_camadas, entradas_camadas, saidas_camadas);
        }
        LMS = LMS/2/param->num_amostras_treinamento;
        delta = abs(LMS - delta);
        delta = LMS;
        printf("\t--------------------------Energia =  %f\r",LMS);
    }
    printf("\n\n\n\tForam necessarias %d epocas.\n\n",i);

    // Libera memoria alocada
    free(indices);

    for (i = 0; i < num_camadas; i++)
        free(saidas_camadas[i]);

    free(saidas_camadas);

    for (i = 0; i < num_camadas; i++)
        free(entradas_camadas[i]);

    free(entradas_camadas);
}
