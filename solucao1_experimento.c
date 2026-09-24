#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REGISTROS 1000
#define MAX_LINHA 2048
#define TAM_CHAVE 100
#define REPETICOES 100000

typedef struct {
    char chave[TAM_CHAVE];
    char linha[MAX_LINHA];
} Registro;

int buscaIterativa(Registro *registros, int quantidade, const char *chave) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(registros[i].chave, chave) == 0) {
            return 1;
        }
    }
    return 0;
}

int buscaRecursiva(Registro *registros, int quantidade, const char *chave, int posicao) {
    if (posicao >= quantidade) {
        return 0;
    }

    if (strcmp(registros[posicao].chave, chave) == 0) {
        return 1;
    }

    return buscaRecursiva(registros, quantidade, chave, posicao + 1);
}

int carregarCSV(const char *nomeArquivo, Registro *registros) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nomeArquivo);
        return -1;
    }

    char linha[MAX_LINHA];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    int quantidade = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (quantidade >= MAX_REGISTROS) {
            break;
        }

        linha[strcspn(linha, "\r\n")] = '\0';

        strcpy(registros[quantidade].linha, linha);

        char copia[MAX_LINHA];
        strcpy(copia, linha);

        char *campo = strtok(copia, ";");

        if (campo != NULL) {
            strcpy(registros[quantidade].chave, campo);
            quantidade++;
        }
    }

    fclose(arquivo);

    return quantidade;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s novos.csv destino.csv\n", argv[0]);
        return 1;
    }

    Registro *destino = malloc(MAX_REGISTROS * sizeof(Registro));
    Registro *novos = malloc(MAX_REGISTROS * sizeof(Registro));

    if (destino == NULL || novos == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(destino);
        free(novos);
        return 1;
    }

    int qtdDestino = carregarCSV(argv[2], destino);
    int qtdNovos = carregarCSV(argv[1], novos);

    if (qtdDestino < 0 || qtdNovos < 0) {
        free(destino);
        free(novos);
        return 1;
    }

    double somaIterativa = 0.0;
    double somaRecursiva = 0.0;

    int resultadoIterativo = 0;
    int resultadoRecursivo = 0;

    for (int repeticao = 0; repeticao < REPETICOES; repeticao++) {

        clock_t inicio = clock();

        for (int i = 0; i < qtdNovos; i++) {
            resultadoIterativo = buscaIterativa(
                destino,
                qtdDestino,
                novos[i].chave
            );
        }

        clock_t fim = clock();

        somaIterativa +=
            (double)(fim - inicio) / CLOCKS_PER_SEC;
    }

    for (int repeticao = 0; repeticao < REPETICOES; repeticao++) {

        clock_t inicio = clock();

        for (int i = 0; i < qtdNovos; i++) {
            resultadoRecursivo = buscaRecursiva(
                destino,
                qtdDestino,
                novos[i].chave,
                0
            );
        }

        clock_t fim = clock();

        somaRecursiva +=
            (double)(fim - inicio) / CLOCKS_PER_SEC;
    }

    double mediaIterativa = somaIterativa / REPETICOES;
    double mediaRecursiva = somaRecursiva / REPETICOES;

    printf("\n========================================\n");
    printf(" EXPERIMENTO - SOLUCAO 1\n");
    printf("========================================\n");

    printf("Registros no destino: %d\n", qtdDestino);
    printf("Novos registros: %d\n", qtdNovos);
    printf("Repeticoes: %d\n\n", REPETICOES);

    printf("BUSCA SEQUENCIAL ITERATIVA\n");
    printf("Tempo medio: %.12f segundos\n", mediaIterativa);

    printf("\nBUSCA SEQUENCIAL RECURSIVA\n");
    printf("Tempo medio: %.12f segundos\n", mediaRecursiva);

    printf("\n========================================\n");
    printf(" RESULTADO\n");
    printf("========================================\n");

    printf("Iterativa: %.12f s\n", mediaIterativa);
    printf("Recursiva: %.12f s\n", mediaRecursiva);

    printf("\nExecucao concluida!\n");

    if (resultadoIterativo || resultadoRecursivo) {
        printf("Buscas executadas com sucesso.\n");
    }

    free(destino);
    free(novos);

    return 0;
}