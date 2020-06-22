#include "treinador.h"
#include "classificador.h"
#include "ler_csv.h"
#include "contar_csv.h"

int main(int argc, char** argv) {
    /*
    argv[0]: Nome do arquivo executavel
    argv[1]: Nro de camadas ocultas
    argv[2]: Tamanho de cada camada oculta, separado por virgula Ex: 4,5,5
    argv[3]: Funcoes de ativacao das camadas ocultas, separado por virgula (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
    argv[4]: Tamanho da ultima camada
    argv[5]: Funcao de ativacao da ultima camada (identidade - 1, sigmoide - 2, tanh - 3, relu - 4, softmax - 5)
    argv[6]: Taxa de aprendizado
    argv[7]: Nro maximo de epocas
    argv[8]: Tolerancia
    argv[9]: Arquivo csv dos dados de treinamento
    argv[10]: Arquivo csv dos dados de teste
    */

    // checar os parametros passados
    if (argc != 11) {
        // Imprime a ajuda
        printf("\nSintaxe de utilizacao:\n");
        printf("-----------------\n");
        printf("Argumento 0: Nome do executavel. Ex: ./perceptron.exe \n");
        printf("Argumento 1: Numero de camadas ocultas. Ex: 3 \n");
        printf("Argumento 2: Tamanho de cada camada oculta, separado por virgula. Ex: 4,5,5 \n");
        printf("Argumento 3: Funcoes de ativacao de cada camada oculta, separadas por virgula. Ex: softmax,relu,tanh \n");
        printf("Argumento 4: Tamanho da ultima camada (1 para classificacao binaria e k para classificacao multi-classes de k-classes). Ex: 1 \n");
        printf("Argumento 5: Funcao de ativacao da ultima camada Ex: sigmoide \n");
        printf("Argumento 6: Taxa de aprendizado Ex: 0.01 \n");
        printf("Argumento 7: Numero maximo de epocas. Ex: 10000 \n");
        printf("Argumento 8: Tolerancia. Ex: 0.01 \n");
        printf("Argumento 9: Caminho do arquivo csv com os dados de treinamento. Ex: dados/dados_treinamento.csv \n");
        printf("Argumento 10: Caminho do arquivo csv com os dados de teste. Ex: dados/dados_teste.csv \n");
        printf("Examplo:\n--------\n~$ ./perceptron.exe 3 4,5,5 softmax,relu,tanh 1 sigmoid 0.01 10000 0.01 dados/dados_treinamento.csv dados/dados_teste.csv\n\n");

        exit(0);
    }

    // Aloca memoria para a estrutura que guarda os parametros
    config_rede *conf = (config_rede *)calloc(1,sizeof(config_rede));
    if(conf == NULL)
    {
        printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
        exit(1);
    }

    // nro de camadas ocultas
    conf->num_camadas_ocultas = atoi(argv[1]);
    // checa se o argumento passado eh valido
    if (conf->num_camadas_ocultas < 0) {
        printf("\n\a\t\tERRO: O numero de camadas ocultas deve ser >= 0\n");
        exit(0);
    }

    // Numero de neuronios em cada camada oculta
    conf->neu_por_camada = (int *)calloc(conf->num_camadas_ocultas, sizeof(int));
    if(conf->neu_por_camada == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    int i;
    char *tok;
    for (i = 0, tok = strtok(argv[2], ","); i < conf->num_camadas_ocultas; i++) {
        conf->neu_por_camada[i] = atoi(tok);
        // checa se o argumento passado eh valido
        if (conf->neu_por_camada[i] <= 0) {
            printf("\n\a\t\tERRO: Os numeros de neuronios das camadas ocultas devem ser positivos\n");
            exit(0);
        }
        tok = strtok(NULL, ",");
    }

    // Funcoes de ativacao de cada camada
    conf->funcs_ativ = (int *)calloc(conf->num_camadas_ocultas, sizeof(int));
    if(conf->funcs_ativ == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    for (i = 0, tok = strtok(argv[3], ","); i < conf->num_camadas_ocultas; i++) {
        if (strcmp(tok, "identidade") == 0) {
            conf->funcs_ativ[i] = 1;
        }
        else if (strcmp(tok, "sigmoide") == 0) {
            conf->funcs_ativ[i] = 2;
        }
        else if (strcmp(tok, "tanh") == 0) {
            conf->funcs_ativ[i] = 3;
        }
        else if (strcmp(tok, "relu") == 0) {
            conf->funcs_ativ[i] = 4;
        }
        else if (strcmp(tok, "softmax") == 0) {
            conf->funcs_ativ[i] = 5;
        }
        else {
            printf("\n\a\t\tERRO: O valor %s eh invalido para a funcao de ativacao de camada oculta %d\n",tok,i+1);
            printf("Escolha entre 'identidade' ou 'sigmoide' ou 'tanh' ou 'relu' ou 'softmax' para funcao de ativacao de camada oculta\n");
            exit(0);
        }

        tok = strtok(NULL, ",");
    }

    // tamanho da ultima camada
    conf->neu_camada_saida = atoi(argv[4]);
    if (conf->neu_camada_saida <= 0) {
        printf("\n\a\t\tERRO: O numero de neuronios da ultima camada deve ser positivo\n");
        exit(0);
    }

    // Funcao de ativacao da ultima camada
    if (strcmp(argv[5], "identidade") == 0) {
        conf->func_ativ_saida = 1;
    }
    else if (strcmp(argv[5], "sigmoide") == 0) {
        conf->func_ativ_saida = 2;
    }
    else if (strcmp(argv[5], "tanh") == 0) {
        conf->func_ativ_saida = 3;
    }
    else if (strcmp(argv[5], "relu") == 0) {
        conf->func_ativ_saida = 4;
    }
    else if (strcmp(argv[5], "softmax") == 0) {
        conf->func_ativ_saida = 5;
    }
    else {
        printf("\n\a\t\tERRO: O valor %s eh invalido para a funcao de ativacao da camada de saida\n",argv[5]);
        printf("Escolha entre 'identidade' ou 'sigmoide' ou 'tanh' ou 'relu' ou 'softmax' para funcao de ativacao da camada de saida\n");
        exit(0);
    }

    // Taxa de aprendizado
    conf->taxa_aprendizado = atof(argv[6]);
    if(conf->taxa_aprendizado <=0 || conf->taxa_aprendizado >= 1){
    	printf("\n\a\t\tERRO: O valor %f da taxa de aprendizado eh invalido\n",conf->taxa_aprendizado);
    	printf("O valor da taxa de aprendizado deve ser 0 < t < 1\n");
    	exit(0);
    }

    // Nro maximo de epocas
    conf->max_epocas = atoi(argv[7]);
    if (conf->max_epocas <= 0) {
        printf("\n\a\t\tERRO: O numero maximo de epocas deve ser positivo\n");
        exit(0);
    }

    // Tolerancia
    conf->tolerancia = atof(argv[8]);
    if (conf->tolerancia <= 0) {
        printf("\n\a\t\tERRO: A tolerancia deve ser positiva\n");
        exit(0);
    }

    // Parametros dos dados de treinamento
    char *arq_treinamento = argv[9];
    contar_csv(arq_treinamento,&conf->num_amostras_treinamento,&conf->num_features);
    // Tamnho das amostras = nro de features + target


    // Cria a matrix dos dados de treinamento
    conf->dados_treinamento = (double **)calloc(conf->num_amostras_treinamento,sizeof(double *));
    if(conf->dados_treinamento == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    for (i = 0; i < conf->num_amostras_treinamento; i++)
    {
        conf->dados_treinamento[i] = (double *)calloc(conf->num_features, sizeof(double));
        if(conf->dados_treinamento[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Le os dados de treinamento do csv para a matriz criada
    ler_csv(arq_treinamento, conf->num_amostras_treinamento, conf->num_features, conf->dados_treinamento);

    // Parametros dos dados de teste
    char *arq_teste = argv[10];
    int temp;
    contar_csv(arq_teste,&conf->num_amostras_teste,&temp);
    // Tamanho das amostras = nro de features + target

    if(conf->num_features != temp){
		printf("\n\a\t\tERRO: O numero de features do arquivo de teste eh diferente do numero de features do arquivo de treinamento!!\n");
		exit(1);
    }

    // Cria a matriz dos dados de teste
    conf->dados_teste = (double **)calloc(conf->num_amostras_teste, sizeof(double *));
    if(conf->dados_teste == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}
    for (i = 0; i < conf->num_amostras_teste; i++)
    {
        conf->dados_teste[i] = (double *)calloc(conf->num_features, sizeof(double));
        if(conf->dados_teste[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    // Le os dados de teste do csv para a matriz criada
    ler_csv(arq_teste, conf->num_amostras_teste, conf->num_features, conf->dados_teste);

    // Nro total de camadas
    int num_camadas = conf->num_camadas_ocultas + 2;

    // Salva os tamanhos das camadas em um array
    int *tam_camada = (int *)calloc(num_camadas, sizeof(int));
    if(tam_camada == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    // tamanho da primeira e da ultima camadas
    tam_camada[0] = conf->num_features - 1;
    tam_camada[num_camadas-1] = conf->neu_camada_saida;

    // tamanhos das camadas ocultas
    for (i = 1; i < num_camadas-1 ; i++)
        tam_camada[i] = conf->neu_por_camada[i-1];

    // Aloca memoria para as matrizes dos pesos
    // cada peso eh um ponteiro para o array de matrizes entre as camadas
    conf->pesos = (double ***)calloc(num_camadas - 1, sizeof(double **));
    if(conf->pesos == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    // Cada matriz entre duas camadas i e i+1 tem tamanho ((layer_size[i]+1) x layer_size[i+1])
    // A matriz de pesos inclui os pesos de bias
    for (i = 0; i < num_camadas-1; i++)
    {
        conf->pesos[i] = (double **)calloc(tam_camada[i]+1, sizeof(double *));
        if(conf->pesos[i] == NULL)
    	{
    		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
    		exit(1);
    	}
    }

    int j;
    for (i = 0; i < num_camadas-1; i++)
    {
        for (j = 0; j < tam_camada[i]+1; j++)
        {
            conf->pesos[i][j] = (double *)calloc(tam_camada[i+1], sizeof(double));
            if(conf->pesos[i][j] == NULL)
        	{
        		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
        		exit(1);
        	}
        }
    }

    // Realiza o treinamento
    printf("Treinando a rede:\n");
    printf("---------\n");
    treinador(conf, tam_camada);
    printf("\nFeito.\n\n");

    // Classifica os dados de teste usando os pesos treinados
    printf("Classificando os dados de teste:\n");
    printf("------------\n");
    classificador(conf, tam_camada);
    printf("\nFeito.\nArquivo de resultados gerado\n");

    // Libera a memoria alocada
    for (i = 0; i < num_camadas-1; i++)
        for (j = 0; j < tam_camada[i]+1; j++)
            free(conf->pesos[i][j]);

    for (i = 0; i < num_camadas-1; i++)
        free(conf->pesos[i]);

    free(conf->pesos);

    free(tam_camada);

    for (i = 0; i < conf->num_amostras_treinamento; i++)
        free(conf->dados_treinamento[i]);

    for (i = 0; i < conf->num_amostras_teste; i++)
        free(conf->dados_teste[i]);

    free(conf->dados_treinamento);
    free(conf->dados_teste);
    free(conf->funcs_ativ);
    free(conf->neu_por_camada);
    free(conf);

    return 0;
}
