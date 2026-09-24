#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REGISTROS 1000
#define MAX_LINHA 2048
#define TAM_CHAVE 100

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

int copiarArquivo(const char *origem, const char *destino) {
    FILE *entrada = fopen(origem, "r");

    if (entrada == NULL) {
        return 0;
    }

    FILE *saida = fopen(destino, "w");

    if (saida == NULL) {
        fclose(entrada);
        return 0;
    }

    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), entrada) != NULL) {
        fputs(linha, saida);
    }

    fclose(entrada);
    fclose(saida);

    return 1;
}

void inserirNoCSV(const char *nomeArquivo, const char *linha) {
    FILE *arquivo = fopen(nomeArquivo, "a");

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "%s\n", linha);

    fclose(arquivo);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s novos.csv destino.csv\n", argv[0]);
        return 1;
    }

    Registro *destino = malloc(MAX_REGISTROS * sizeof(Registro));
    Registro *novos = malloc(MAX_REGISTROS * sizeof(Registro));
    Registro *destinoIterativo = malloc(MAX_REGISTROS * sizeof(Registro));
    Registro *destinoRecursivo = malloc(MAX_REGISTROS * sizeof(Registro));

    if (destino == NULL || novos == NULL ||
        destinoIterativo == NULL || destinoRecursivo == NULL) {

        printf("Erro ao alocar memoria.\n");

        free(destino);
        free(novos);
        free(destinoIterativo);
        free(destinoRecursivo);

        return 1;
    }

    int qtdDestino = carregarCSV(argv[2], destino);

    if (qtdDestino < 0) {
        free(destino);
        free(novos);
        free(destinoIterativo);
        free(destinoRecursivo);
        return 1;
    }

    int qtdNovos = carregarCSV(argv[1], novos);

    if (qtdNovos < 0) {
        free(destino);
        free(novos);
        free(destinoIterativo);
        free(destinoRecursivo);
        return 1;
    }

    printf("\n========================================\n");
    printf(" SOLUCAO 1 - BUSCA SEQUENCIAL\n");
    printf("========================================\n");

    printf("Registros no destino: %d\n", qtdDestino);
    printf("Novos registros: %d\n\n", qtdNovos);

    memcpy(
        destinoIterativo,
        destino,
        qtdDestino * sizeof(Registro)
    );

    int qtdIterativa = qtdDestino;
    int inseridosIterativa = 0;

    clock_t inicioIterativa = clock();

    for (int i = 0; i < qtdNovos; i++) {

        int encontrado = buscaIterativa(
            destinoIterativo,
            qtdIterativa,
            novos[i].chave
        );

        if (!encontrado && qtdIterativa < MAX_REGISTROS) {

            destinoIterativo[qtdIterativa] = novos[i];

            qtdIterativa++;
            inseridosIterativa++;
        }
    }

    clock_t fimIterativa = clock();

    double tempoIterativa =
        (double)(fimIterativa - inicioIterativa)
        / CLOCKS_PER_SEC;

    printf("BUSCA ITERATIVA\n");
    printf("Registros inseridos: %d\n", inseridosIterativa);
    printf("Tempo: %.9f segundos\n\n", tempoIterativa);

    memcpy(
        destinoRecursivo,
        destino,
        qtdDestino * sizeof(Registro)
    );

    int qtdRecursiva = qtdDestino;
    int inseridosRecursiva = 0;

    clock_t inicioRecursiva = clock();

    for (int i = 0; i < qtdNovos; i++) {

        int encontrado = buscaRecursiva(
            destinoRecursivo,
            qtdRecursiva,
            novos[i].chave,
            0
        );

        if (!encontrado && qtdRecursiva < MAX_REGISTROS) {

            destinoRecursivo[qtdRecursiva] = novos[i];

            qtdRecursiva++;
            inseridosRecursiva++;
        }
    }

    clock_t fimRecursiva = clock();

    double tempoRecursiva =
        (double)(fimRecursiva - inicioRecursiva)
        / CLOCKS_PER_SEC;

    printf("BUSCA RECURSIVA\n");
   printf("Registros inseridos: %d\n", inseridosRecursiva);
    printf("Tempo: %.9f segundos\n\n", tempoRecursiva);

    copiarArquivo(
        argv[2],
        "resultado_iterativo.csv"
    );

    for (int i = qtdDestino; i < qtdIterativa; i++) {
        inserirNoCSV(
            "resultado_iterativo.csv",
            destinoIterativo[i].linha
        );
    }

    copiarArquivo(
        argv[2],
        "resultado_recursivo.csv"
    );

    for (int i = qtdDestino; i < qtdRecursiva; i++) {
        inserirNoCSV(
            "resultado_recursivo.csv",
            destinoRecursivo[i].linha
        );
    }

    printf("Arquivos gerados:\n");
    printf("- resultado_iterativo.csv\n");
    printf("- resultado_recursivo.csv\n");

    printf("\n========================================\n");
    printf(" Execucao concluida!\n");
    printf("========================================\n");

    free(destino);
    free(novos);
    free(destinoIterativo);
    free(destinoRecursivo);

    return 0;
}