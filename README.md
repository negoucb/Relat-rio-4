# Análise de Algoritmos – Inserção de Registros em CSV

## Descrição

Este projeto apresenta a implementação e análise de algoritmos para inserção de novos registros em um arquivo CSV, garantindo que não sejam inseridos registros com valores duplicados na coluna definida como chave de unicidade.

O trabalho foi desenvolvido em linguagem C e utiliza duas soluções diferentes para verificar a existência de registros.

## Base de Dados

A base utilizada foi **Agentes Econômicos Estrangeiros**, disponível no Portal de Dados Abertos do Governo Federal:

https://dados.gov.br/dados/conjuntos-dados/agentes-economicos-estrangeiros

A base possui:

- 106 registros;
- 9 colunas;
- Valores únicos na coluna `REGISTRO_ANCINE`.

A coluna `REGISTRO_ANCINE` foi utilizada como chave de unicidade.

## Solução 1 – Busca Sequencial

A primeira solução utiliza busca sequencial para verificar se a chave do novo registro já existe no arquivo de destino.

Foram implementadas duas versões:

- Busca sequencial iterativa;
- Busca sequencial recursiva.

### Complexidade

| Algoritmo | Tempo | Espaço |
|---|---|---|
| Busca sequencial iterativa | O(n) | O(1) |
| Busca sequencial recursiva | O(n) | O(n) |

## Solução 2 – Ordenação e Busca Binária

A segunda solução utiliza os valores da chave armazenados em um vetor na memória.

Primeiramente, o vetor é ordenado utilizando MergeSort. Depois, a existência das chaves é verificada utilizando busca binária.

Foram implementadas:

- MergeSort iterativo;
- MergeSort recursivo;
- Busca binária iterativa;
- Busca binária recursiva.

### Complexidade

| Algoritmo | Tempo | Espaço |
|---|---|---|
| MergeSort iterativo | O(n log n) | O(n) |
| MergeSort recursivo | O(n log n) | O(n) |
| Busca binária iterativa | O(log n) | O(1) |
| Busca binária recursiva | O(log n) | O(log n) |

## Arquivos

- `solucao1.c` – implementação da Solução 1.
- `solucao2.c` – implementação da Solução 2.
- `solucao1_experimento.c` – versão utilizada para os testes experimentais da Solução 1.
- `solucao2_experimento.c` – versão utilizada para os testes experimentais da Solução 2.
- `destino.csv` – arquivo CSV utilizado como destino.
- `novos.csv` – arquivo contendo os novos registros para inserção.

## Testes

Os experimentos foram realizados com 100.000 repetições para obter tempos de execução mais representativos.

Foram testadas as versões iterativas e recursivas dos algoritmos.

Nos arquivos de teste, existem registros com chaves novas e registros com chaves já existentes. Assim, o programa verifica quais registros podem ser inseridos e quais devem ser rejeitados.

Resultado esperado:

- 2 registros novos inseridos;
- 2 registros duplicados rejeitados.

## Compilação

É necessário possuir o compilador GCC instalado.

No terminal, dentro da pasta do projeto, execute:

```bash
gcc solucao1.c -o solucao1.exe
gcc solucao2.c -o solucao2.exe
gcc solucao1_experimento.c -o solucao1_experimento.exe
gcc solucao2_experimento.c -o solucao2_experimento.exe
