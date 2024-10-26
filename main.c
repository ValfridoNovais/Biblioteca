#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define FILENAME "C:\\Repositorios_GitHube\\MeusProjetos\\Biblioteca\\biblioteca.csv"

// Estrutura que define os campos do livro
typedef struct {
    int indice;
    char titulo[100];
    char autor[100];
    int ano;
    int lido;  // 1 para lido, 0 para não lido
    int doado; // 1 para doado, 0 para não doado
} Livro;

// Lista dinâmica para armazenar os livros
typedef struct {
    Livro *livros;
    int tamanho;
    int capacidade;
} Biblioteca;

// Declaração da função liberarBiblioteca
void liberarBiblioteca(Biblioteca *biblioteca);

// Função para inicializar a biblioteca
void inicializarBiblioteca(Biblioteca *biblioteca) {
    biblioteca->tamanho = 0;
    biblioteca->capacidade = 1;
    biblioteca->livros = malloc(biblioteca->capacidade * sizeof(Livro));
}

// Função para carregar livros do arquivo CSV
void carregarLivros(Biblioteca *biblioteca) {
    printf("Carregando livros do arquivo...\n");

    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo CSV. Verifique o caminho e o arquivo.\n");
        return;
    }

    // Ignora a primeira linha do cabeçalho
    char linha[256];
    if (fgets(linha, sizeof(linha), file) == NULL) {
        printf("Arquivo CSV vazio ou formato incorreto.\n");
        fclose(file);
        return;
    }

    // Leitura dos dados de cada livro
    while (fgets(linha, sizeof(linha), file)) {
        Livro livro;
        int resultado = sscanf(linha, "%d;%99[^;];%99[^;];%d;%d;%d",
                               &livro.indice, livro.titulo, livro.autor,
                               &livro.ano, &livro.lido, &livro.doado);

        // Verifica se a linha foi lida corretamente
        if (resultado == 6) {
            if (biblioteca->tamanho >= biblioteca->capacidade) {
                biblioteca->capacidade += 1;
                biblioteca->livros = realloc(biblioteca->livros, biblioteca->capacidade * sizeof(Livro));
            }
            biblioteca->livros[biblioteca->tamanho++] = livro;
        } else {
            printf("Erro ao ler a linha: %s\n", linha);
        }
    }
    fclose(file);
    printf("Carregamento concluído.\n");
}

// Função para salvar todos os livros no arquivo CSV (reescreve o arquivo)
void salvarTodosLivrosNoArquivo(Biblioteca *biblioteca) {
    FILE *file = fopen(FILENAME, "w");
    fprintf(file, "Indice;Titulo;Autor;Ano;Lido;Doado\n");
    for (int i = 0; i < biblioteca->tamanho; i++) {
        Livro livro = biblioteca->livros[i];
        fprintf(file, "%d;%s;%s;%d;%d;%d\n", livro.indice, livro.titulo, livro.autor, livro.ano, livro.lido, livro.doado);
    }
    fclose(file);
}

// Função para adicionar um livro à biblioteca
void adicionarLivro(Biblioteca *biblioteca, char *titulo, char *autor, int ano, int lido) {
    if (biblioteca->tamanho >= biblioteca->capacidade) {
        biblioteca->capacidade += 1;
        biblioteca->livros = realloc(biblioteca->livros, biblioteca->capacidade * sizeof(Livro));
    }
    Livro novoLivro;
    novoLivro.indice = biblioteca->tamanho == 0 ? 1 : biblioteca->livros[biblioteca->tamanho - 1].indice + 1;
    strcpy(novoLivro.titulo, titulo);
    strcpy(novoLivro.autor, autor);
    novoLivro.ano = ano;
    novoLivro.lido = lido;
    novoLivro.doado = 0;

    biblioteca->livros[biblioteca->tamanho++] = novoLivro;
    salvarTodosLivrosNoArquivo(biblioteca);
    printf("Livro adicionado com sucesso!\n");
}

// Função para marcar um livro como doado
void doarLivro(Biblioteca *biblioteca, int indice) {
    for (int i = 0; i < biblioteca->tamanho; i++) {
        if (biblioteca->livros[i].indice == indice && biblioteca->livros[i].doado == 0) {
            biblioteca->livros[i].doado = 1;
            salvarTodosLivrosNoArquivo(biblioteca);
            printf("Livro marcado como doado com sucesso!\n");
            return;
        }
    }
    printf("Livro não encontrado ou já doado.\n");
}

// Função para exibir todos os livros
void exibirLivros(Biblioteca *biblioteca) {
    printf("Lista de Livros:\n");
    for (int i = 0; i < biblioteca->tamanho; i++) {
        printf("%d. %s, por %s (%d) - %s - %s\n", biblioteca->livros[i].indice,
               biblioteca->livros[i].titulo,
               biblioteca->livros[i].autor,
               biblioteca->livros[i].ano,
               biblioteca->livros[i].doado ? "Doado" : "Disponível",
               biblioteca->livros[i].lido ? "Lido" : "Não Lido");
    }
}

// Função para liberar a memória da biblioteca
void liberarBiblioteca(Biblioteca *biblioteca) {
    free(biblioteca->livros);
}

// Função principal que exibe o menu interativo
void menu() {
    setlocale(LC_ALL, "Portuguese");

    Biblioteca biblioteca;
    inicializarBiblioteca(&biblioteca);
    carregarLivros(&biblioteca);

    int opcao, ano, lido, indice;
    char titulo[100], autor[100];

    while (1) {
        printf("\n--- Menu da Biblioteca ---\n");
        printf("1. Adicionar Livro\n");
        printf("2. Exibir Livros\n");
        printf("3. Marcar Livro como Doado\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        switch(opcao) {
            case 1:
                printf("Digite o título: ");
                scanf(" %[^\n]", titulo);
                printf("Digite o autor: ");
                scanf(" %[^\n]", autor);
                printf("Digite o ano: ");
                if (scanf("%d", &ano) != 1) {
                    printf("Ano inválido! Tente novamente.\n");
                    while (getchar() != '\n');
                    continue;
                }
                printf("Já leu este livro? (1 para sim, 0 para não): ");
                if (scanf("%d", &lido) != 1) {
                    printf("Entrada inválida! Tente novamente.\n");
                    while (getchar() != '\n');
                    continue;
                }
                adicionarLivro(&biblioteca, titulo, autor, ano, lido);
                break;
            case 2:
                exibirLivros(&biblioteca);
                printf("\nPressione Enter para voltar ao menu...\n");
                while (getchar() != '\n');
                getchar();
                break;
            case 3:
                printf("Digite o índice do livro para marcar como doado: ");
                if (scanf("%d", &indice) != 1) {
                    printf("Índice inválido! Tente novamente.\n");
                    while (getchar() != '\n');
                    continue;
                }
                doarLivro(&biblioteca, indice);
                break;
            case 4:
                liberarBiblioteca(&biblioteca);
                printf("Saindo...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
                while (getchar() != '\n');
        }
    }
}

int main() {
    menu();
    return 0;
