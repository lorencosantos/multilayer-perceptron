#include "classificador.h"

#define max(x, y) (x > y ? x : y)

void multiplica_matriz_classf(double *a, double **b, double *resultado, int n, int p) {
    // vetor a de tamanho 1 x n
    // matriz b de tamanho n x p
    // vetor resultante de tamanho 1 x p
    // resultado = a * b
    int j, k;
    for (j = 0; j < p; j++) {
        resultado[j] = 0.0;
        for (k = 0; k < n; k++)
            resultado[j] += (a[k] * b[k][j]);
    }
}

void identidade_classif(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = entrada[i]; // funcao identidade
}

void sigmoide_classif(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = 1.0 / (1.0 + exp(-entrada[i])); // funcao sigmoide
}

void tan_h_classif(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = tanh(entrada[i]); // funcao tanh
}

void relu_classif(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = max(0.0, entrada[i]); // funcao ReLU
}

void softmax_classif(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    double sum = 0.0;
    for (i = 0; i < n; i++)
        sum += exp(entrada[i]);

    for (i = 0; i < n; i++)
        saida[i+1] = exp(entrada[i]) / sum; // funcao softmax
}

void classificador(config_rede *param, int *tam_camadas) {
    int num_camadas = param->num_camadas_ocultas + 2;

    // ALoca memoria para os arrays das entradas das camadas
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
        saidas_camadas[i] = (double *)calloc(tam_camadas[i]+1, sizeof(double));
        if(saidas_camadas[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Aloca memoria para guardar as saidas
    double **saida_final = (double **)calloc(param->num_amostras_teste, sizeof(double *));
    if(saida_final == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    for (i = 0; i < param->num_amostras_teste; i++)
    {
        saida_final[i] = (double *)calloc(param->neu_camada_saida, sizeof(double));
        if(saida_final[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }


    // Classifica os dados de teste
    int amostra_atual;
    for (amostra_atual = 0; amostra_atual < param->num_amostras_teste; amostra_atual++) {
        //printf("Classificando a amostra de teste %d de %d\r", amostra_atual+1, param->num_amostras_teste);
        // Coloca a amostra de teste na entrada e saida da primeira camada
        saidas_camadas[0][0] = 1; // bias da primeira camada
        for (i = 0; i < param->num_features-1; i++)
            saidas_camadas[0][i+1] = entradas_camadas[0][i] = param->dados_teste[amostra_atual][i];

        // Realiza a propagacao direta para cada camada oculta
        // Calcula a entrada e a saida de cada camada oculta
        for (i = 1; i < num_camadas-1; i++) {
            // Calcula as entradas da camada [i]
            multiplica_matriz_classf(saidas_camadas[i-1], param->pesos[i-1], entradas_camadas[i], tam_camadas[i-1]+1, tam_camadas[i]);

            // Calcula as saidas da camada [i]
            // Funcoes de ativacao (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
            switch (param->funcs_ativ[i-1]) {
                case 1: // identidade
                    identidade_classif(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                    break;
                case 2: // sigmoide
                    sigmoide_classif(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                    break;
                case 3: // tanh
                    tan_h_classif(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                    break;
                case 4: // relu
                    relu_classif(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                    break;
                case 5: // softmax
                    softmax_classif(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                    break;
                default:
                    printf("Propagacao direta: Funcao de ativacao de camada oculta invalida\n");
                    exit(0);
                    break;
            }
        }

        // Preenche a entrada e a saida da ultima camada
        multiplica_matriz_classf(saidas_camadas[num_camadas-2], param->pesos[num_camadas-2], entradas_camadas[num_camadas-1], tam_camadas[num_camadas-2]+1, tam_camadas[num_camadas-1]);

        // Funcoes de ativacao da ultima camada (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
        switch (param->func_ativ_saida) {
            case 1: // identidade
                identidade_classif(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
                break;
            case 2: // sigmoide
                sigmoide_classif(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
                break;
            case 3: // tanh
                tan_h_classif(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
                break;
            case 4: // relu
                relu_classif(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
                break;
            case 5: // softmax
                softmax_classif(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
                break;
            default:
                printf("Propagacao direta: Funcao de ativacao da ultima camada invalida\n");
                exit(0);
                break;
        }

        // Salva a saida calculada na matriz de saida
        // A saida final esta em saidas_camadas[n_layers-1] a partir do indice 1
        for (i = 0; i < param->neu_camada_saida; i++)
            saida_final[amostra_atual][i] = saidas_camadas[num_camadas-1][i+1];

    }

    // Encontra a classe da saida para cada amostra de teste
    if (param->neu_camada_saida == 1) { // classificacao binaria
        for (amostra_atual = 0; amostra_atual < param->num_amostras_teste; amostra_atual++) {
            if (saida_final[amostra_atual][0] < 0.5)
                saida_final[amostra_atual][0] = 0;
            else
                saida_final[amostra_atual][0] = 1;
        }
    }
    else { // Classificacao multi-classe
        for (amostra_atual = 0; amostra_atual < param->num_amostras_teste; amostra_atual++) {
            double max = -1;
            int max_class;
            for (i = 0; i < param->neu_camada_saida; i++) {
                if (saida_final[amostra_atual][i] > max) {
                    max = saida_final[amostra_atual][i];
                    max_class = i+1;
                }
            }
            saida_final[amostra_atual][0] = max_class;
        }
    }

    // Calcula a matriz de confusao
    if (param->neu_camada_saida == 1) { // Classificacao binaria
        int verdadeiro_positivo = 0, verdadeiro_negativo = 0, falso_positivo = 0, falso_negativo = 0;
        for (amostra_atual = 0; amostra_atual < param->num_amostras_teste; amostra_atual++) {
            if ((int)saida_final[amostra_atual][0] == 0) {
                if ((int)param->dados_teste[amostra_atual][param->num_features-1] == 0)
                    ++verdadeiro_negativo;
                else
                    ++falso_negativo;
            }
            else {
                if ((int)param->dados_teste[amostra_atual][param->num_features-1] == 1)
                    ++verdadeiro_positivo;
                else
                    ++falso_positivo;
            }
           // printf("Caso %d: %f %f \n",amostra_atual, param->dados_teste[amostra_atual][param->num_features-1],saida_final[amostra_atual][0]);
        }

        // Calcula a acuracia
        double acuracia = (double)(verdadeiro_positivo + verdadeiro_negativo) / param->num_amostras_teste;

        // Escreve a matriz de confusao
        printf("\n\nMatriz de confusao\n");
        printf("-----------------\n\n");

        printf("\t    |predito 0\t predito 1\n");
        printf("--------------------------------------------\n");
        printf("Real 0    |%d\t\t%d\n\n", verdadeiro_negativo, falso_positivo);
        printf("Real 1    |%d\t\t%d\n\n", falso_negativo, verdadeiro_positivo);

        // Imprime a acuracia
        printf("\nAcuracia: %.2lf\n\n", acuracia * 100);
    }
    else { // Classificacao multi-classe
        int **matriz_confusao = (int **)calloc(param->neu_camada_saida, sizeof(int *));
        if(matriz_confusao == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
        for (i = 0; i < param->neu_camada_saida; i++)
        {
            matriz_confusao[i] = (int *)calloc(param->neu_camada_saida, sizeof(int));
            if(matriz_confusao[i] == NULL)
        	{
        		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
        		exit(1);
        	}
        }


        // preenche a matriz de confusao
        int classe_real, classe_predita;
        for (amostra_atual = 0; amostra_atual < param->num_amostras_teste; amostra_atual++) {
            classe_real = param->dados_teste[amostra_atual][param->num_features-1] - 1;
            classe_predita = saida_final[amostra_atual][0] - 1;

            ++matriz_confusao[classe_real][classe_predita];
        }

        // imprime a matriz de confusao
        printf("\t");
        for (classe_predita = 1; classe_predita <= param->neu_camada_saida; classe_predita++)
            printf("Predito %d  ", classe_predita);
        printf("\n---------------------------------------------------------------------------\n");

        for (classe_real = 0; classe_real < param->neu_camada_saida; classe_real++) {
            printf("Real %d | ", classe_real+1);
            for (classe_predita = 0; classe_predita < param->neu_camada_saida; classe_predita++)
                printf("%d\t", matriz_confusao[classe_real][classe_predita]);
            printf("\n");
        }

        // Encontra a acuracia
        double acuracia = 0.0;
        for (i = 0; i < param->neu_camada_saida; i++)
            acuracia += matriz_confusao[i][i];
        acuracia /= param->num_amostras_teste;

        // Imprime a acuracia
        printf("\nAcuracia: %.2lf\n\n", acuracia * 100);

        // Libera memoria alocada
        for (i = 0; i < param->neu_camada_saida; i++)
            free(matriz_confusao[i]);
        free(matriz_confusao);
    }


    // Escreve a saida final em um csv
    char *arq_saida = "dados/saida_teste.csv";
    escrever_csv(arq_saida, param->num_amostras_teste, param->neu_camada_saida, saida_final);

    // Libera a memoria alocada
    for (i = 0; i < param->num_amostras_teste; i++)
        free(saida_final[i]);

    free(saida_final);

    for (i = 0; i < num_camadas; i++)
        free(saidas_camadas[i]);

    free(saidas_camadas);

    for (i = 0; i < num_camadas; i++)
        free(entradas_camadas[i]);

    free(entradas_camadas);
}
