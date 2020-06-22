#include "forward_propagation.h"

#define max(x, y) (x > y ? x : y)

void multiplica_matriz(double *a, double **b, double *resultado, int n, int p) {
    // vetor a de tamanho 1 x n
    // matriz b de tamanho n x p
    // vetor result de tamanho 1 x p
    // resultado = a * b
    int j, k;
    for (j = 0; j < p; j++) {
        resultado[j] = 0.0;
        for (k = 0; k < n; k++)
            resultado[j] += (a[k] * b[k][j]);
    }
}

void identidade(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = entrada[i]; // funcao identidade
}

void sigmoide(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = 1.0 / (1.0 + exp(-entrada[i])); // funcao sigmoide
}

void tan_h(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = tanh(entrada[i]); // funcao tanh
}

void relu(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    for (i = 0; i < n; i++)
        saida[i+1] = max(0.0, entrada[i]); // funcao ReLU
}

void softmax(int n, double *entrada, double *saida) {
    saida[0] = 1; // bias

    int i;
    double sum = 0.0;
    for (i = 0; i < n; i++)
        sum += exp(entrada[i]);

    for (i = 0; i < n; i++)
        saida[i+1] = exp(entrada[i]) / sum; // funcao softmax
}

void forward_propagation(config_rede *param, int amostra_atual, int num_camadas, int *tam_camadas, double **entradas_camadas, double **saidas_camadas) {
    // Preenche a entrada e a saida da primeira camada com os dados
    int i;
    saidas_camadas[0][0] = 1; // bias da primeira camada
    for (i = 0; i < param->num_features-1; i++)
        saidas_camadas[0][i+1] = entradas_camadas[0][i] = param->dados_treinamento[amostra_atual][i];

    // Realiza a propagacao direta para cada camada oculta
    // Calcula a entrada e a saida de cada camada oculta
    for (i = 1; i < num_camadas-1; i++) {
        // Calcula a entrada da camada [i]
        multiplica_matriz(saidas_camadas[i-1], param->pesos[i-1], entradas_camadas[i], tam_camadas[i-1]+1, tam_camadas[i]);

        // Calcula a saida da camada [i]
        // Funcoes de ativacao (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
        switch (param->funcs_ativ[i-1]) {
            case 1: // identidade
                identidade(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                break;
            case 2: // sigmoide
                sigmoide(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                break;
            case 3: // tanh
                tan_h(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                break;
            case 4: // relu
                relu(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                break;
            case 5: // softmax
                softmax(tam_camadas[i], entradas_camadas[i], saidas_camadas[i]);
                break;
            default:
                printf("\n\a\t\tERRO NA PROPAGACAO DIRETA: Funcao de ativacao da camada oculta %d invalida\n", i);
                exit(0);
                break;
        }
    }

    // Preenche a entrada e a saida da ultima camada
    multiplica_matriz(saidas_camadas[num_camadas-2], param->pesos[num_camadas-2], entradas_camadas[num_camadas-1], tam_camadas[num_camadas-2]+1, tam_camadas[num_camadas-1]);

    // Funcoes de ativacao da ultima camada (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
    switch (param->func_ativ_saida) {
        case 1: // identidade
            identidade(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
            break;
        case 2: // sigmoide
            sigmoide(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
            break;
        case 3: // tanh
            tan_h(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
            break;
        case 4: // relu
            relu(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
            break;
        case 5: // softmax
            softmax(tam_camadas[num_camadas-1], entradas_camadas[num_camadas-1], saidas_camadas[num_camadas-1]);
            break;
        default:
            printf("\n\a\t\tERRO NA PROPAGACAO DIRETA: Funcao de ativacao da ultima camada invalida\n");
            exit(0);
            break;
    }
}
