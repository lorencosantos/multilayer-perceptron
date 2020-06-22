#include "contar_csv.h"

void contar_csv(char *arq, int *linhas, int *cols) {
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

    *linhas = 0;
	*cols = 0;
	while(fgets(linha, MAX_LINE_SIZE, fp)) {
		*linhas += 1;
		if (*linhas == 1) {
			char *tok = strtok(linha, ",");
			while(tok){
				tok = strtok(NULL, ",\n");
				*cols += 1;
			}
			continue;
		}

		char *tok = strtok(linha, ",");
		while(tok){
			tok = strtok(NULL, ",\n");
		}
	}

    // Libera memoria alocada
    free(linha);

    // Fecha o arquivo
    fclose(fp);
}
