#include "back_propagation.h"

void d_identitidade(int num_neuronios, double *entrada, double *saida, double *derivada) {
    int i;
    for (i = 0; i < num_neuronios; i++)
        derivada[i] = 1;
}

void d_sigmoide(int num_neuronios, double *entrada, double *saida, double *derivada) {
    int i;
    for (i = 0; i < num_neuronios; i++)
        derivada[i] = saida[i+1] * (1.0 - saida[i+1]);
}

void d_tanh(int num_neuronios, double *entrada, double *saida, double *derivada) {
    int i;
    for (i = 0; i < num_neuronios; i++)
        derivada[i] = 1.0 - saida[i+1] * saida[i+1];
}

void d_relu(int num_neuronios, double *entrada, double *saida, double *derivada) {
    int i;
    for (i = 0; i < num_neuronios; i++) {
        if (entrada[i] > 0)
            derivada[i] = 1;
        else if (entrada[i] < 0)
            derivada[i] = 0;
        else // a derivada nao existe
            derivada[i] = 0.5; // valor arbitrario
    }
}

void d_softmax(int num_neuronios, double *entrada, double *saida, double *derivada) {
    int i;
    for (i = 0; i < num_neuronios; i++)
        derivada[i] = saida[i+1] * (1.0 - saida[i+1]);
}

void calcula_gradiente_local(config_rede *param, int camada_atual, int num_camadas, int *tam_camadas, double **entradas_camadas, double **saidas_camadas, double *saidas_esperadas, double **gradiente_local) {
    // Aloca memoria para as derivadas
    double **derivadas_camadas = (double **)calloc(num_camadas, sizeof(double *));
    if(derivadas_camadas == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    int i;
    for (i = 0; i < num_camadas; i++)
    {
        derivadas_camadas[i] = (double *)calloc(tam_camadas[i], sizeof(double));
        if(derivadas_camadas[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Se for a ultima camada
    if (camada_atual == num_camadas-1) {
        // erro produzido na ultima camada
        double *erro_saida = (double *)calloc(param->neu_camada_saida, sizeof(double));
        if(erro_saida == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}

        for (i = 0; i < param->neu_camada_saida; i++)
        {
            erro_saida[i] = saidas_esperadas[i] - saidas_camadas[camada_atual][i+1];
        }

        // Calcula as derivadas das camadas
        // Calcula os gradientes locais
        switch(param->func_ativ_saida) {
            case 1: // identidade
                d_identitidade(param->neu_camada_saida, entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < param->neu_camada_saida; i++)
                    gradiente_local[camada_atual][i] = erro_saida[i] * derivadas_camadas[camada_atual][i];

                break;
            case 2: // sigmoide
                d_sigmoide(param->neu_camada_saida, entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < param->neu_camada_saida; i++)
                    gradiente_local[camada_atual][i] = erro_saida[i] * derivadas_camadas[camada_atual][i];

                break;
            case 3: // tanh
                d_tanh(param->neu_camada_saida, entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < param->neu_camada_saida; i++)
                    gradiente_local[camada_atual][i] = erro_saida[i] * derivadas_camadas[camada_atual][i];

                break;
            case 4: // relu
                d_relu(param->neu_camada_saida, entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < param->neu_camada_saida; i++)
                    gradiente_local[camada_atual][i] = erro_saida[i] * derivadas_camadas[camada_atual][i];

                break;
            case 5: // softmax
                d_softmax(param->neu_camada_saida, entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < param->neu_camada_saida; i++)
                    gradiente_local[camada_atual][i] = erro_saida[i] * derivadas_camadas[camada_atual][i];

                break;
            default:
                printf("\n\a\t\tERRO: Calculo do gradiente local: Funcao de ativacao invalida da ultima camada\n");
                exit(0);
                break;
        }

        // Libera a memoria alocada
        free(erro_saida);
    }
    else { // se for uma camada escondida
        // calcula as derivadas da camada para todas as unidades
        // calcula o gradiente local
        int j;
        switch (param->funcs_ativ[camada_atual-1]) {
            case 1: // identidade
                d_identitidade(tam_camadas[camada_atual], entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < tam_camadas[camada_atual]; i++) {
                    double erro = 0.0;
                    for (j = 0; j < tam_camadas[camada_atual+1]; j++)
                        erro += gradiente_local[camada_atual+1][j] * param->pesos[camada_atual][i][j];

                    gradiente_local[camada_atual][i] = erro * derivadas_camadas[camada_atual][i];
                }

                break;
            case 2: // sigmoide
                d_sigmoide(tam_camadas[camada_atual], entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < tam_camadas[camada_atual]; i++) {
                    double erro = 0.0;
                    for (j = 0; j < tam_camadas[camada_atual+1]; j++)
                        erro += gradiente_local[camada_atual+1][j] * param->pesos[camada_atual][i][j];

                    gradiente_local[camada_atual][i] = erro * derivadas_camadas[camada_atual][i];
                }

                break;
            case 3: // tanh
                d_tanh(tam_camadas[camada_atual], entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < tam_camadas[camada_atual]; i++) {
                    double erro = 0.0;
                    for (j = 0; j < tam_camadas[camada_atual+1]; j++)
                        erro += gradiente_local[camada_atual+1][j] * param->pesos[camada_atual][i][j];

                    gradiente_local[camada_atual][i] = erro * derivadas_camadas[camada_atual][i];
                }

                break;
            case 4: // relu
                d_relu(tam_camadas[camada_atual], entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < tam_camadas[camada_atual]; i++) {
                    double erro = 0.0;
                    for (j = 0; j < tam_camadas[camada_atual+1]; j++)
                        erro += gradiente_local[camada_atual+1][j] * param->pesos[camada_atual][i][j];

                    gradiente_local[camada_atual][i] = erro * derivadas_camadas[camada_atual][i];
                }

                break;
            case 5: // softmax
                d_softmax(tam_camadas[camada_atual], entradas_camadas[camada_atual], saidas_camadas[camada_atual], derivadas_camadas[camada_atual]);

                for (i = 0; i < tam_camadas[camada_atual]; i++) {
                    double erro = 0.0;
                    for (j = 0; j < tam_camadas[camada_atual+1]; j++)
                        erro += gradiente_local[camada_atual+1][j] * param->pesos[camada_atual][i][j];

                    gradiente_local[camada_atual][i] = erro * derivadas_camadas[camada_atual][i];
                }

                break;
            default:
                printf("\n\a\t\tERRO: Funcao de ativacao invalida da camada escondida\n");
                exit(0);
                break;
        }
    }

    // Libera a memoria alocada
    for (i = 0; i < num_camadas; i++)
        free(derivadas_camadas[i]);

    free(derivadas_camadas);

}

void back_propagation(config_rede *param, int amostra_atual, int num_camadas, int *tam_camadas, double **entradas_camadas, double **saidas_camadas) {
    /* ------------------ Saida esperada ----------------------------------------*/
    // Pega a saida esperada da matriz de dados
    // Aloca memoria para o array de saidas esperadas
    // Inizializa ele como zeros
    double *saidas_esperadas = (double *)calloc(param->neu_camada_saida, sizeof(double));
    if(saidas_esperadas == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    // Cria uma mascara para o elemento respectivo no array saidas_esperadas
    // Ex: se y = 3 e neu_camada_saida = 4 entao saidas_esperadas = [0, 0, 1, 0]
    if (param->neu_camada_saida == 1)
        saidas_esperadas[0] = param->dados_treinamento[amostra_atual][param->num_features-1];
    else
        saidas_esperadas[(int)(param->dados_treinamento[amostra_atual][param->num_features-1] - 1)] = 1;

    /* ---------------------- Alocacao de memoria para a correcao dos pesos ----------------------------------- */
    // Aloca memoria para as matrizes de correcao dos pesos entre as camadas
    // correcao_pesos eh um ponteiro para uma matriz entre as camadas
    double ***correcao_pesos = (double ***)calloc(num_camadas-1, sizeof(double **));
    if(correcao_pesos == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    // Cada matriz entre camadas i e i+1 tem tamanho ((tam_camadas[i]+1) x tam_camadas[i+1])
    // A matriz correcao_pesos inclui as correcoes dos bias
    int i;
    for (i = 0; i < num_camadas-1; i++)
    {
        correcao_pesos[i] = (double **)calloc(tam_camadas[i]+1, sizeof(double *));
        if(correcao_pesos[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    int j;
    for (i = 0; i < num_camadas-1; i++)
        for (j = 0; j < tam_camadas[i]+1; j++)
        {
            correcao_pesos[i][j] = (double *)calloc(tam_camadas[i+1], sizeof(double));
            if(correcao_pesos[i][j] == NULL)
        	{
        		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
        		exit(1);
        	}
        }

    /* --------------------- Aloca memoria para o gradiente local --------------------------------------*/
    // Aloca memoria para o gradiente local de cada camada
    double **gradiente_local = (double **)calloc(num_camadas, sizeof(double *));
    if(gradiente_local == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    for (i = 0; i < num_camadas; i++)
    {
        gradiente_local[i] = (double *)calloc(tam_camadas[i], sizeof(double));
        if(gradiente_local[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    /*----------- Calcula a correcao dos pesos para todas as camadas -------------------*/
    // Correcao dos pesos para a camada de saida
    calcula_gradiente_local(param, num_camadas-1, num_camadas, tam_camadas, entradas_camadas, saidas_camadas, saidas_esperadas, gradiente_local);
    for (i = 0; i < param->neu_camada_saida; i++)
        for (j = 0; j < tam_camadas[num_camadas-2]+1; j++)
            correcao_pesos[num_camadas-2][j][i] = (param->taxa_aprendizado) * gradiente_local[num_camadas-1][i] * saidas_camadas[num_camadas-2][j];

    // Correcao dos pesos para as camadas ocultas
    int k;
    for (i = num_camadas-2; i >= 1; i--) {
        calcula_gradiente_local(param, i, num_camadas, tam_camadas, entradas_camadas, saidas_camadas, saidas_esperadas, gradiente_local);

        for (j = 0; j < tam_camadas[i]; j++)
            for (k = 0; k < tam_camadas[i-1]+1; k++)
                correcao_pesos[i-1][k][j] = (param->taxa_aprendizado) * gradiente_local[i][j] * saidas_camadas[i-1][k];
    }

    /*----------------- Atualiza os pesos -------------------------------------*/
    for (i = 0; i < num_camadas-1; i++) {
        for (j = 0; j < tam_camadas[i]+1; j++) {
            for (k = 0; k < tam_camadas[i+1]; k++) {
                param->pesos[i][j][k] += correcao_pesos[i][j][k];
            }
        }
    }


    // Libera a memoria alocada
    for (i = 0; i < num_camadas; i++)
        free(gradiente_local[i]);

    free(gradiente_local);

    for (i = 0; i < num_camadas - 1; i++)
        for (j = 0; j < tam_camadas[i]+1; j++)
            free(correcao_pesos[i][j]);

    for (i = 0; i < num_camadas - 1; i++)
        free(correcao_pesos[i]);

    free(correcao_pesos);

    free(saidas_esperadas);
}
