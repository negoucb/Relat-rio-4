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

int buscaBinariaRecursiva(
    Registro *v,
    int inicio,
    int fim,
    const char *chave
) {
    if (inicio > fim) {
        return 0;
    }

    int meio = (inicio + fim) / 2;

    int comparacao = strcmp(v[meio].chave, chave);

    if (comparacao == 0) {
        return 1;
    }

    if (comparacao < 0) {
        return buscaBinariaRecursiva(
            v,
            meio + 1,
            fim,
            chave
        );
    }

    return buscaBinariaRecursiva(
        v,
        inicio,
        meio - 1,
        chave
    );
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

    Registro *destino = malloc(
        MAX_REGISTROS * sizeof(Registro)
    );

    Registro *novos = malloc(
        MAX_REGISTROS * sizeof(Registro)
    );

    Registro *vetorIterativo = malloc(
        MAX_REGISTROS * sizeof(Registro)
    );

    Registro *vetorRecursivo = malloc(
        MAX_REGISTROS * sizeof(Registro)
    );

    if (destino == NULL ||
        novos == NULL ||
        vetorIterativo == NULL ||
        vetorRecursivo == NULL) {

        printf("Erro ao alocar memoria.\n");

        free(destino);
        free(novos);
        free(vetorIterativo);
        free(vetorRecursivo);

        return 1;
    }

    int qtdDestino = carregarCSV(
        argv[2],
        destino
    );

    int qtdNovos = carregarCSV(
        argv[1],
        novos
    );

    if (qtdDestino < 0 || qtdNovos < 0) {

        free(destino);
        free(novos);
        free(vetorIterativo);
        free(vetorRecursivo);

        return 1;
    }

    double somaMergeIterativo = 0.0;
    double somaMergeRecursivo = 0.0;
    double somaBuscaIterativa = 0.0;
    double somaBuscaRecursiva = 0.0;

    int resultado = 0;

    for (int repeticao = 0;
         repeticao < REPETICOES;
         repeticao++) {

        memcpy(
            vetorIterativo,
            destino,
            qtdDestino * sizeof(Registro)
        );

        clock_t inicio = clock();

        mergeSortIterativo(
            vetorIterativo,
            qtdDestino
        );

        clock_t fim = clock();

        somaMergeIterativo +=
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;
    }

    for (int repeticao = 0;
         repeticao < REPETICOES;
         repeticao++) {

        memcpy(
            vetorRecursivo,
            destino,
            qtdDestino * sizeof(Registro)
        );

        Registro *temp = malloc(
            qtdDestino * sizeof(Registro)
        );

        clock_t inicio = clock();

        mergeSortRecursivo(
            vetorRecursivo,
            temp,
            0,
            qtdDestino - 1
        );

        clock_t fim = clock();

        somaMergeRecursivo +=
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;

        free(temp);
    }

    mergeSortIterativo(
        vetorIterativo,
        qtdDestino
    );

    mergeSortRecursivo(
        vetorRecursivo,
        malloc(qtdDestino * sizeof(Registro)),
        0,
        qtdDestino - 1
    );

    for (int repeticao = 0;
         repeticao < REPETICOES;
         repeticao++) {

        clock_t inicio = clock();

        for (int i = 0; i < qtdNovos; i++) {
            resultado = buscaBinariaIterativa(
                vetorIterativo,
                qtdDestino,
                novos[i].chave
            );
        }

        clock_t fim = clock();

        somaBuscaIterativa +=
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;
    }

    for (int repeticao = 0;
         repeticao < REPETICOES;
         repeticao++) {

        clock_t inicio = clock();

        for (int i = 0; i < qtdNovos; i++) {
            resultado = buscaBinariaRecursiva(
                vetorRecursivo,
                0,
                qtdDestino - 1,
                novos[i].chave
            );
        }

        clock_t fim = clock();

        somaBuscaRecursiva +=
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;
    }

    double mediaMergeIterativo =
        somaMergeIterativo / REPETICOES;

    double mediaMergeRecursivo =
        somaMergeRecursivo / REPETICOES;

    double mediaBuscaIterativa =
        somaBuscaIterativa / REPETICOES;

    double mediaBuscaRecursiva =
        somaBuscaRecursiva / REPETICOES;

    printf("\n========================================\n");
    printf(" EXPERIMENTO - SOLUCAO 2\n");
    printf("========================================\n");

    printf("Registros no destino: %d\n", qtdDestino);
    printf("Novos registros: %d\n", qtdNovos);
    printf("Repeticoes: %d\n\n", REPETICOES);

    printf("MERGESORT ITERATIVO\n");
    printf("Tempo medio: %.12f segundos\n\n",
           mediaMergeIterativo);

    printf("MERGESORT RECURSIVO\n");
    printf("Tempo medio: %.12f segundos\n\n",
           mediaMergeRecursivo);

    printf("BUSCA BINARIA ITERATIVA\n");
    printf("Tempo medio: %.12f segundos\n\n",
           mediaBuscaIterativa);

    printf("BUSCA BINARIA RECURSIVA\n");
    printf("Tempo medio: %.12f segundos\n\n",
           mediaBuscaRecursiva);

    printf("========================================\n");
    printf(" RESULTADO\n");
    printf("========================================\n");

    printf("MergeSort iterativo: %.12f s\n",
           mediaMergeIterativo);

    printf("MergeSort recursivo: %.12f s\n",
           mediaMergeRecursivo);

    printf("Busca binaria iterativa: %.12f s\n",
           mediaBuscaIterativa);

    printf("Busca binaria recursiva: %.12f s\n",
           mediaBuscaRecursiva);

    printf("\nExecucao concluida!\n");

    if (resultado) {
        printf("Buscas executadas com sucesso.\n");
    }

    free(destino);
    free(novos);
    free(vetorIterativo);
    free(vetorRecursivo);

    return 0;
}