#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// STRUCT P/ Filme !!! TORNAR TAM DA STRING DINAMICO
typedef struct filme
{
    char generos[50][50];
    char nome[50];
    int duracao;
    int anoLancamento;
    char sinopse[120];
    int avaliacao;
} Filme;

// Declarando no c/ dado FILME
typedef struct noFilme
{
    Filme dado;
    struct no *proxNo;
} tipoNoFilme;

// Declarando listaCircular
typedef struct
{
    tipoNoFilme *fim;
    int quant;
} tipoListaFilme;

// STRUCT P GENERO
typedef struct genero // TORNAR TAM DA STRING DINAMICO
{
    char nome[50];
    int qntdFilmes;
    char descricao[120];
} Genero;

// Nó de genero, dup encadeada, proxNoGen, e NoFilme apontand
typedef struct noGenero
{
    Genero dado;
    struct no *proxNo, *proxNoFilme;
} tipoNoGenero;

typedef struct lista
{
    tipoNoGenero *fim, *inicio;
    int quant;
} tipoListaGenero;

// utilizar antes de receber a string, obviamente serve para limpar possiveis \n antes da leitura de uma string SOLUÇÃO CLASSUDA DUK
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// precisa comentar?
void inicializarListaFilmes(tipoListaFilme *lista)
{
    lista->fim = NULL;
    lista->quant = 0;
}
void inicializarListaGenero(tipoListaGenero *lista)
{
    lista->fim = NULL;
    lista->inicio = NULL;
    lista->quant = 0;
}

// precisa mesmo comentar?
int inserirGeneroListaVazia(tipoListaGenero *lista, Genero dado)
{
    return 0;
}

// precisa mesmo comentar?
int inserirFilmeListaVazia(tipoListaFilme *lista, Filme filme)
{
    return 0;
}

// Seria um diferencial inserir em ordem alfabetica
int inserirGenero(tipoListaGenero *lista, Genero valor)
{
    return 0;
}

// Diferencial inserir em ordem alfabetica
int inserirFilme(tipoListaFilme *lista, Filme filme)
{
    return 0;
}

int removerGenero()
{
    return 0;
}

int removerFilme()
{
    return 0;
}

int procurarGeneroPorNome()
{
    return 0;
}

int procurarFilmePorNome()
{
    return 0;
}

void exibeListaGenero(tipoListaGenero *lista)
{
}

void exibeListaFilme(tipoListaFilme *lista)
{
}

int main()
{
    int resp, op;
    Genero genero;
    Filme filme;
    tipoListaGenero *listaGeneros = (tipoListaGenero *)malloc(sizeof(tipoListaGenero));
    inicializarListaGenero(listaGeneros);
    tipoListaFilme *listaFilmes = (tipoListaFilme *)malloc(sizeof(tipoListaFilme));
    inicializarListaFilmes(listaFilmes);
    printf("Bem-vindo a UFSFlix\n");
    do
    {
        printf("\n1 - Inserir Genero\n2 - Remover Genero\n3 - Exibir Filmes por Genero\n4 - Procurar Genero");
        printf("\n5 - Inserir Filme\n6 - Remover Filme\n7 - Exibir Filme\n8 - Procurar Filme\n9 - Pedir recomendacao de Filme");
        printf("\n0 - Sair do programa");
        printf("\nDigite sua opcao: ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            break;
        case 0:
            printf("\nEncerrando programa.\n");
            break;
        default:
            printf("\n\nOpcao indisponivel. Selecione uma nova opcao.\n");
            break;
        }
    } while (op != 0);
    return 0;
}