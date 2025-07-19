#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM 9

int mensagensErroAtivadas = 1;

// Funções principais
void imprimirTabuleiro(int tabuleiro[TAM][TAM]);
int interpretarEntrada(int tabuleiro[TAM][TAM], char entrada[], int *coluna, int *linha, int *valor, char nomeArquivo[]);
void salvarTabuleiro(int tabuleiro[TAM][TAM], char nomeArquivo[]);
int validarJogada(int tabuleiro[TAM][TAM], int fixos[TAM][TAM], int coluna, int linha, int valor);
int celulaFixa(int fixos[TAM][TAM], int linha, int coluna);
int valorNaLinha(int tabuleiro[TAM][TAM], int linha, int valor);
int valorNaColuna(int tabuleiro[TAM][TAM], int coluna, int valor);
int valorNaRegiao(int tabuleiro[TAM][TAM], int linha, int coluna, int valor);
void verificarVitoria(int tabuleiro[TAM][TAM], int *valorFinal);
void lerJogadas(int tabuleiro[TAM][TAM], int fixos[TAM][TAM], char nomeArquivo[]);

// Impressão formatada do tabuleiro
void imprimirTabuleiro(int tabuleiro[TAM][TAM]) {

    printf("     1  2  3   4  5  6   7  8  9\n");
    printf("   +---------+---------+---------+\n");

    for (int i = 0; i < TAM; i++) {
        printf(" %d |", i + 1);  // Numeração da linha

        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0)
                printf(" . ");
            else
                printf(" %d ", tabuleiro[i][j]);

            if ((j + 1) % 3 == 0)
                printf("|");
        }

        printf("\n");

        if ((i + 1) % 3 == 0)
            printf("   +---------+---------+---------+\n");
    }
}

// Entrada de comandos do usuário
void lerJogadas(int tabuleiro[TAM][TAM], int fixos[TAM][TAM], char nomeArquivo[]) {
    char entrada[20];
    int coluna, linha, valor;

    do {
        scanf("%s", entrada);
        int comando = interpretarEntrada(tabuleiro, entrada, &coluna, &linha, &valor, nomeArquivo);

        if (comando > 0) {
            int jogadaValida = validarJogada(tabuleiro, fixos, coluna, linha, valor);
            if (comando != 0 && jogadaValida == 0) {
                tabuleiro[linha][coluna] = valor;
                imprimirTabuleiro(tabuleiro);
            }
        }

        verificarVitoria(tabuleiro, &valor);

    } while (valor != 42);
}

// Interpreta entrada do usuário
int interpretarEntrada(int tabuleiro[TAM][TAM], char entrada[], int *coluna, int *linha, int *valor, char nomeArquivo[]) {
    if (strcmp(entrada, "sair") == 0) {
        *valor = 42;
        return 0;
    } else if (strcmp(entrada, "salvar") == 0) {
        salvarTabuleiro(tabuleiro, nomeArquivo);
        return 0;
    } else {
        *linha = entrada[0] - '0' - 1;
        *coluna = entrada[1] - '0' - 1;
        *valor = entrada[2] - '0';
        return 1;
    }
}

// Salva o tabuleiro
void salvarTabuleiro(int tabuleiro[TAM][TAM], char nomeArquivo[]) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            fprintf(arquivo, "%d ", tabuleiro[i][j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
    printf("Jogo salvo com sucesso!\n");
}

// Valida se a jogada pode ser feita
int validarJogada(int tabuleiro[TAM][TAM], int fixos[TAM][TAM], int coluna, int linha, int valor) {
    int ocupada = celulaFixa(fixos, linha, coluna);
    int naLinha = valorNaLinha(tabuleiro, linha, valor);
    int naColuna = valorNaColuna(tabuleiro, coluna, valor);
    int naRegiao = valorNaRegiao(tabuleiro, linha, coluna, valor);

    return ocupada || naLinha || naColuna || naRegiao;
}

int celulaFixa(int fixos[TAM][TAM], int linha, int coluna) {
    if (fixos[linha][coluna] != 0) {
        if (mensagensErroAtivadas)
            printf("Erro! A celula %d,%d ja possui valor fixo.\n", linha + 1, coluna + 1);
        return 1;
    }
    return 0;
}

int valorNaLinha(int tabuleiro[TAM][TAM], int linha, int valor) {
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[linha][i] == valor) {
            if (mensagensErroAtivadas)
                printf("Erro! A linha %d ja tem o valor %d na coluna %d.\n", linha + 1, valor, i + 1);
            return 1;
        }
    }
    return 0;
}

int valorNaColuna(int tabuleiro[TAM][TAM], int coluna, int valor) {
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[i][coluna] == valor) {
            if (mensagensErroAtivadas)
                printf("Erro! A coluna %d ja tem o valor %d na linha %d.\n", coluna + 1, valor, i + 1);
            return 1;
        }
    }
    return 0;
}

int valorNaRegiao(int tabuleiro[TAM][TAM], int linha, int coluna, int valor) {
    int linIni = linha - linha % 3;
    int colIni = coluna - coluna % 3;

    for (int i = linIni; i < linIni + 3; i++) {
        for (int j = colIni; j < colIni + 3; j++) {
            if (tabuleiro[i][j] == valor) {
                if (mensagensErroAtivadas)
                    printf("Erro! A regiao 3x3 ja contem o valor %d na posicao %d,%d.\n", valor, i + 1, j + 1);
                return 1;
            }
        }
    }
    return 0;
}

// Detecta vitória
void verificarVitoria(int tabuleiro[TAM][TAM], int *valorFinal) {
    int preenchido = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (tabuleiro[i][j] != 0)
                preenchido++;

    if (preenchido == 81) {
        printf("\n VITORIA! Voce completou o Sudoku!\n");
        *valorFinal = 42;
    }
}

// Função principal com geração aleatória
int main() {
    int tabuleiro[TAM][TAM] = {0};
    int fixos[TAM][TAM] = {0};
    char nomeArquivo[] = "sudoku_salvo.txt";

    srand(time(NULL));

    mensagensErroAtivadas = 0; // Desativa prints durante geração
    int gerados = 0;
    while (gerados < 15) {
        int linha = rand() % TAM;
        int coluna = rand() % TAM;
        int valor = (rand() % 9) + 1;

        if (fixos[linha][coluna] == 0 &&
            valorNaLinha(tabuleiro, linha, valor) == 0 &&
            valorNaColuna(tabuleiro, coluna, valor) == 0 &&
            valorNaRegiao(tabuleiro, linha, coluna, valor) == 0) {

            tabuleiro[linha][coluna] = valor;
            fixos[linha][coluna] = valor;
            gerados++;
        }
    }
    mensagensErroAtivadas = 1; // Reativa prints

    printf("=== BEM-VINDO ===\n");
    printf("Digite uma jogada no formato: <linha> <coluna> <valor>\n");
    printf("Ex: 231: linha 2, coluna 3, valor 1\n");
    printf("Digite 'salvar' para salvar o jogo ou 'sair' para encerrar.\n");

    imprimirTabuleiro(tabuleiro);
    lerJogadas(tabuleiro, fixos, nomeArquivo);

    printf("Obrigado por jogar!\n");
    return 0;
}