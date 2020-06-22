#include "ler_csv.h"

void ler_csv(char *arq, int linhas, int cols, double **dados) {
    // abre o arquivo e testa a validade
    FILE *fp = fopen(arq, "r");
    if (NULL == fp) {
        printf("Erro ao abrir o arquivo %s. Certifique-se que o caminho do arquivo esta correto\n", arq);
        exit(0);
    }

    // Aloca memoria para ler uma linha/coluna do arquivo
    char *linha = (char *)malloc(MAX_LINE_SIZE * sizeof(char));
    if(linha == NULL)
	{
		printf("\n\a\t\tERRO: NAO FOI POSSIVEL ALOCAR MEMORIA!!\n");
		exit(1);
	}

    // Le o arquivo linha por linha e salva na matriz 'data'
    int i, j;
    for (i = 0; fgets(linha, MAX_LINE_SIZE, fp) && i < linhas; i++) {
        char *tok = strtok(linha, ",");
        for (j = 0; tok && *tok; j++) {
            dados[i][j] = atof(tok);
            tok = strtok(NULL, ",\n");
        }
    }

    // Libera memoria alocada
    free(linha);

    // Fecha o arquivo
    fclose(fp);
}
