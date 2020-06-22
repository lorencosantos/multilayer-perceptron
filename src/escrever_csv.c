#include "escrever_csv.h"

void escrever_csv(char *arq, int linhas, int cols, double **dados) {
    FILE *fp = fopen(arq, "w");
    if (NULL == fp) {
        printf("Nao foi possivel criar/abrir o arquivo %s. Certifique-se que voce tem permissao para criar/abrir um arquivo nesse diretorio\n", arq);
        exit(0);
    }

    // Cria um cabecalho no arquivo com os numeros da da ultima camada
    int i;
    for (i = 1; i <= cols-1; i++)
        fprintf(fp, "Saida do neuronio %d,", i);
    fprintf(fp, "Saida do neuronio %d\n", cols);

    // Despeja a matriz no arquivo, elemento por elemento
    int j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j <= cols-2; j++) {
            fprintf(fp, "%lf,", dados[i][j]);
        }
        fprintf(fp, "%lf\n", dados[i][cols-1]);
    }

    // Fecha o arquivo
    fclose(fp);
}
