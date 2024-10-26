#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que define os campos do livro
typedef struct {
    char titulo[100];
    char autor[100];
    int ano;
} Livro;

// Lista din�mica para armazenar os livros
typedef struct {
    Livro *livros;
    int tamanho;
    int capacidade;
} Biblioteca;

// Fun��o para limpar a tela
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fun��o para inicializar a biblioteca sem limite fixo inicial
void inicializarBiblioteca(Biblioteca *biblioteca) {
    biblioteca->tamanho = 0;
    biblioteca->capacidade = 1; // Come�amos com capacidade 1
    biblioteca->livros = malloc(biblioteca->capacidade * sizeof(Livro));
}

// Fun��o para adicionar um livro � biblioteca com capacidade incremental
void adicionarLivro(Biblioteca *biblioteca, char *titulo, char *autor, int ano) {
    // Incrementa a capacidade em 1 se necess�rio
    if (biblioteca->tamanho >= biblioteca->capacidade) {
        biblioteca->capacidade += 1;
        biblioteca->livros = realloc(biblioteca->livros, biblioteca->capacidade * sizeof(Livro));
    }
    // Adiciona o livro
    strcpy(biblioteca->livros[biblioteca->tamanho].titulo, titulo);
    strcpy(biblioteca->livros[biblioteca->tamanho].autor, autor);
    biblioteca->livros[biblioteca->tamanho].ano = ano;
    biblioteca->tamanho++;
    printf("Livro adicionado com sucesso!\n");
}

// Fun��o para exibir todos os livros
void exibirLivros(Biblioteca *biblioteca) {
    printf("Lista de Livros:\n");
    int i;
    for (i = 0; i < biblioteca->tamanho; i++) {
        printf("%d. %s, por %s (%d)\n", i + 1, biblioteca->livros[i].titulo, biblioteca->livros[i].autor, biblioteca->livros[i].ano);
    }
}

// Fun��o para remover um livro pelo �ndice
void removerLivro(Biblioteca *biblioteca, int indice) {
    if (indice < 0 || indice >= biblioteca->tamanho) {
        printf("�ndice inv�lido!\n");
        return;
    }
    int i;
    for (i = indice; i < biblioteca->tamanho - 1; i++) {
        biblioteca->livros[i] = biblioteca->livros[i + 1];
    }
    biblioteca->tamanho--;
    printf("Livro removido com sucesso!\n");
}

// Fun��o para liberar a mem�ria da biblioteca
void liberarBiblioteca(Biblioteca *biblioteca) {
    free(biblioteca->livros);
}

// Fun��o principal que exibe o menu interativo
void menu() {
    Biblioteca biblioteca;
    inicializarBiblioteca(&biblioteca);

    int opcao, ano, indice;
    char titulo[100], autor[100];

    while (1) {
        limparTela(); // Limpa a tela a cada itera��o do menu
        printf("\n--- Menu da Biblioteca ---\n");
        printf("1. Adicionar Livro\n");
        printf("2. Exibir Livros\n");
        printf("3. Remover Livro\n");
        printf("4. Sair\n");
        printf("Escolha uma op��o: ");
        
        // Verifica se a entrada � v�lida
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inv�lida! Pressione Enter para tentar novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }

        switch(opcao) {
            case 1:
                printf("Digite o t�tulo: ");
                scanf(" %[^\n]", titulo);
                printf("Digite o autor: ");
                scanf(" %[^\n]", autor);
                printf("Digite o ano: ");
                if (scanf("%d", &ano) != 1) {
                    printf("Ano inv�lido! Pressione Enter para tentar novamente.\n");
                    while (getchar() != '\n'); // Limpa o buffer de entrada
                    continue;
                }
                adicionarLivro(&biblioteca, titulo, autor, ano);
                break;
            case 2:
                exibirLivros(&biblioteca);
                printf("\nPressione Enter para voltar ao menu...\n");
                while (getchar() != '\n'); // Limpa o buffer de entrada
                getchar(); // Espera o usu�rio pressionar Enter
                break;
            case 3:
                printf("Digite o �ndice do livro para remover: ");
                if (scanf("%d", &indice) != 1) {
                    printf("�ndice inv�lido! Pressione Enter para tentar novamente.\n");
                    while (getchar() != '\n'); // Limpa o buffer de entrada
                    continue;
                }
                removerLivro(&biblioteca, indice - 1);
                printf("\nPressione Enter para voltar ao menu...\n");
                while (getchar() != '\n'); // Limpa o buffer de entrada
                getchar(); // Espera o usu�rio pressionar Enter
                break;
            case 4:
                liberarBiblioteca(&biblioteca);
                printf("Saindo...\n");
                return;
            default:
                printf("Op��o inv�lida! Pressione Enter para tentar novamente.\n");
                while (getchar() != '\n'); // Limpa o buffer de entrada
        }
    }
}

int main() {
    menu();
    return 0;
}
