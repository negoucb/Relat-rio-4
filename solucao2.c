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

void mesclar(Registro *v, Registro *temp, int inicio, int meio, int fim) {
    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        if (strcmp(v[i].chave, v[j].chave) <= 0) {
            temp[k++] = v[i++];
        } else {
            temp[k++] = v[j++];
        }
    }

    while (i <= meio) {
        temp[k++] = v[i++];
    }

    while (j <= fim) {
        temp[k++] = v[j++];
    }

    for (i = inicio; i <= fim; i++) {
        v[i] = temp[i];
    }
}

void mergeSortRecursivo(Registro *v, Registro *temp, int inicio, int fim) {
    if (inicio >= fim) {
        return;
    }

    int meio = (inicio + fim) / 2;

    mergeSortRecursivo(v, temp, inicio, meio);
    mergeSortRecursivo(v, temp, meio + 1, fim);

    mesclar(v, temp, inicio, meio, fim);
}

void mergeSortIterativo(Registro *v, int n) {
    Registro *temp = malloc(n * sizeof(Registro));

    if (temp == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    for (int tamanho = 1; tamanho < n; tamanho *= 2) {
        for (int inicio = 0; inicio < n - 1; inicio += 2 * tamanho) {

            int meio = inicio + tamanho - 1;
            int fim = inicio + 2 * tamanho - 1;

            if (meio >= n - 1) {
                continue;
            }

            if (fim >= n) {
                fim = n - 1;
            }

            mesclar(v, temp, inicio, meio, fim);
        }
    }

    free(temp);
}

int buscaBinariaIterativa(Registro *v, int n, const char *chave) {
    int inicio = 0;
    int fim = n - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        int comparacao = strcmp(v[meio].chave, chave);

        if (comparacao == 0) {
            return 1;
        }

        if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return 0;
}

int buscaBinariaRecursiva(Registro *v, int inicio, int fim, const char *chave) {
    if (inicio > fim) {
        return 0;
    }

    int meio = (inicio + fim) / 2;

    int comparacao = strcmp(v[meio].chave, chave);

    if (comparacao == 0) {
        return 1;
    }

    if (comparacao < 0) {
        return buscaBinariaRecursiva(v, meio + 1, fim, chave);
    }

    return buscaBinariaRecursiva(v, inicio, meio - 1, chave);
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

    printf("\n========================================\n");
    printf(" SOLUCAO 2 - ORDENACAO E BUSCA BINARIA\n");
    printf("========================================\n");

    printf("Registros no destino: %d\n", qtdDestino);
    printf("Novos registros: %d\n\n", qtdNovos);

    Registro *vetorIterativo = malloc(MAX_REGISTROS * sizeof(Registro));
    Registro *vetorRecursivo = malloc(MAX_REGISTROS * sizeof(Registro));

    memcpy(
        vetorIterativo,
        destino,
        qtdDestino * sizeof(Registro)
    );

    memcpy(
        vetorRecursivo,
        destino,
        qtdDestino * sizeof(Registro)
    );

    clock_t inicioMergeIterativo = clock();

    mergeSortIterativo(vetorIterativo, qtdDestino);

    clock_t fimMergeIterativo = clock();

    double tempoMergeIterativo =
        (double)(fimMergeIterativo - inicioMergeIterativo)
        / CLOCKS_PER_SEC;

    clock_t inicioMergeRecursivo = clock();

    Registro *temp = malloc(qtdDestino * sizeof(Registro));

    mergeSortRecursivo(
        vetorRecursivo,
        temp,
        0,
        qtdDestino - 1
    );

    free(temp);

    clock_t fimMergeRecursivo = clock();

    double tempoMergeRecursivo =
        (double)(fimMergeRecursivo - inicioMergeRecursivo)
        / CLOCKS_PER_SEC;

    printf("MERGESORT ITERATIVO\n");
    printf("Tempo: %.9f segundos\n\n", tempoMergeIterativo);

    printf("MERGESORT RECURSIVO\n");
    printf("Tempo: %.9f segundos\n\n", tempoMergeRecursivo);

    int inseridosIterativo = 0;
    int inseridosRecursivo = 0;

    clock_t inicioBuscaIterativa = clock();

    for (int i = 0; i < qtdNovos; i++) {

        int encontrado = buscaBinariaIterativa(
            vetorIterativo,
            qtdDestino,
            novos[i].chave
        );

        if (!encontrado) {
            inseridosIterativo++;
        }
    }

    clock_t fimBuscaIterativa = clock();

    double tempoBuscaIterativa =
        (double)(fimBuscaIterativa - inicioBuscaIterativa)
        / CLOCKS_PER_SEC;

    clock_t inicioBuscaRecursiva = clock();

    for (int i = 0; i < qtdNovos; i++) {

        int encontrado = buscaBinariaRecursiva(
            vetorRecursivo,
            0,
            qtdDestino - 1,
            novos[i].chave
        );

        if (!encontrado) {
            inseridosRecursivo++;
        }
    }

    clock_t fimBuscaRecursiva = clock();

    double tempoBuscaRecursiva =
        (double)(fimBuscaRecursiva - inicioBuscaRecursiva)
        / CLOCKS_PER_SEC;

    printf("BUSCA BINARIA ITERATIVA\n");
    printf("Registros novos: %d\n", inseridosIterativo);
    printf("Tempo: %.9f segundos\n\n", tempoBuscaIterativa);

    printf("BUSCA BINARIA RECURSIVA\n");
    printf("Registros novos: %d\n", inseridosRecursivo);
    printf("Tempo: %.9f segundos\n\n", tempoBuscaRecursiva);

    FILE *arquivo = fopen(
        "resultado_solucao2.csv",
        "w"
    );

    if (arquivo != NULL) {

        FILE *original = fopen(argv[2], "r");

        if (original != NULL) {

            char linha[MAX_LINHA];

            while (fgets(linha, sizeof(linha), original) != NULL) {
                fputs(linha, arquivo);
            }

            fclose(original);
        }

        for (int i = 0; i < qtdNovos; i++) {

            int encontrado = buscaBinariaIterativa(
                vetorIterativo,
                qtdDestino,
                novos[i].chave
            );

            if (!encontrado) {
                inserirNoCSV(
                    "resultado_solucao2.csv",
                    novos[i].linha
                );
            }
        }

        fclose(arquivo);
    }

    printf("Arquivo gerado:\n");
    printf("- resultado_solucao2.csv\n");

    printf("\n========================================\n");
    printf(" Execucao concluida!\n");
    printf("========================================\n");

    free(destino);
    free(novos);
    free(vetorIterativo);
    free(vetorRecursivo);

    return 0;
}