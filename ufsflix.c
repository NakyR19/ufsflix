#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_DESC 220
#define MAX_SINOPSE 250
#define MAX_COMMENT 200

// struct filme
typedef struct filme
{
    char nome[MAX_NOME];
    int duracao;
    int anoLancamento;
    char sinopse[MAX_SINOPSE];
    int avaliacao; // 0 a 5 estrelas?????? tratar isso dps
} Filme;

// nó da lista circ
typedef struct noFilme
{
    Filme dado;
    struct noFilme *proxNo;
} tipoNoFilme;

// lista circular
typedef struct listaFilme
{
    tipoNoFilme *fim; // ponteiro para o último nó (obviamente fim->prox é o primeiro)
    int quant;
} tipoListaFilme;

// struc do genero
typedef struct genero
{
    char nome[MAX_NOME];
    int qntdFilmes;
    char descricao[MAX_DESC];
} Genero;

// nó da lista duplamente encadeada
typedef struct noGenero
{
    Genero dado;
    struct noGenero *proxNo;
    struct noGenero *antNo; // excluir????? tratar isso dps
    struct noFilme *proxNoFilme;
} tipoNoGenero;

// lista dup encadeada
typedef struct listaGenero
{
    tipoNoGenero *inicio;
    tipoNoGenero *fim;
    int quant;
} tipoListaGenero;

// filme assistido - EXTRA
typedef struct filmeAssistido
{
    Filme dado;
    int avaliacaoPessoal;
    char comentario[MAX_COMMENT];
    int favorito; // 0 = Não, 1 = Sim
} FilmeAssistido;

// nó p/ lista de filmes assistidos
typedef struct noAssistido
{
    FilmeAssistido dado;
    struct noAssistido *prox;
} tipoNoAssistido;

// lista de filmes assistidos
typedef struct listaAssistidos
{
    tipoNoAssistido *inicio;
    int quant;
} tipoListaAssistidos;

// ----------------------------------------------------------
// -----------Funções para o tratamento de string------------
// ----------------------------------------------------------

// utilizar antes de receber a string, obviamente serve para limpar possiveis \n antes da leitura de uma string
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Converte a string p/ maiuscula
void padronizarString(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
}

// Remove o \n de uma string lida pelo fgets
void removeQuebraLinha(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

// -------------------------------------------
// -----------Funções para o Gênero-----------
// -------------------------------------------

// precisa comentar?
void inicializarListaGenero(tipoListaGenero *lista)
{
    lista->fim = NULL;
    lista->inicio = NULL;
    lista->quant = 0;
}

// precisa mesmo comentar?
// Ordem alfabética de preferencia, caso não consiga pode ir normalmente, SEJA FELIZ
void inserirGeneroEmListaVazia(ListaGeneros *lista, char *genero) {
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->genero, genero);
    novo->anterior = lista->cabeca;
    novo->proximo = lista->cabeca;
    lista->cabeca->proximo = novo;
    lista->cabeca->anterior = novo;
}
// Ordem alfabética de preferencia, caso não consiga pode ir normalmente, SEJA FELIZ

void inserirGenero(ListaGeneros *lista, char *genero) {
    // Verifica se o gênero já existe na lista
    No *atual = lista->cabeca->proximo;
    while (atual != lista->cabeca) {
        if (strcmp(atual->genero, genero) == 0) {
            printf("Gênero '%s' já existe na lista.\n", genero);
            return;
        }
        atual = atual->proximo;
    }

    // Cria um novo nó
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->genero, genero);

    // Insere em ordem alfabética
    atual = lista->cabeca->proximo;
    while (atual != lista->cabeca && strcmp(atual->genero, genero) < 0) {
        atual = atual->proximo;
    }

    // Ajusta os ponteiros
    novo->proximo = atual;
    novo->anterior = atual->anterior;
    atual->anterior->proximo = novo;
    atual->anterior = novo;
}

No *buscarGenero(ListaGeneros *lista, char *genero) {
    No *atual = lista->cabeca->proximo;
    while (atual != lista->cabeca) {
        if (strcmp(atual->genero, genero) == 0) {
            return atual; // Retorna o nó encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Gênero não encontrado
}

// Remove um gênero da lista (libera também a lista de filmes associada)
// retorna 1 se foi removido com sucesso, 0 se não existe, uau q novidade
int removerGenero(ListaGeneros *lista, char *genero) {
    // Busca o gênero na lista
    No *atual = buscarGenero(lista, genero);

    if (atual == NULL) {
        printf("Gênero '%s' não encontrado.\n", genero);
        return 0; // Gênero não encontrado
    }

    // Ajusta os ponteiros dos nós adjacentes
    atual->anterior->proximo = atual->proximo;
    atual->proximo->anterior = atual->anterior;

    // Libera a memória do nó removido
    free(atual);

    printf("Gênero '%s' removido com sucesso.\n", genero);
    return 1; // Gênero removido
}
void exibirGeneros(ListaGeneros *lista) {
    No *atual = lista->cabeca->proximo;
    if (atual == lista->cabeca) {
        printf("Nenhum gênero cadastrado.\n");
        return;
    }

    printf("Gêneros cadastrados:\n");
    while (atual != lista->cabeca) {
        printf("- %s\n", atual->genero);
        atual = atual->proximo;
    }
}
// -------------------------------------------
// -----------Funções para o filme-----------
// -------------------------------------------

// precisa mesmo comentar?
void inicializarListaFilme(tipoListaFilme *lista)
{
    lista->fim = NULL;
    lista->quant = 0;
}

// Ordem alfabética de preferencia, caso não consiga pode ir normalmente, SEJA FELIZ
int inserirFilmeEmListaVazia(tipoNoGenero *noGen, Filme filme)
{
    // LEMBRA DE TRATAR A STRING PELO AMOR 
    return 0;
}

// Ordem alfabética de preferencia, caso não consiga pode ir normalmente, SEJA FELIZ
int inserirFilmeNaLista(tipoNoGenero *noGen, Filme filme)
{
    // LEMBRA DE TRATAR A STRING PELO AMOR 
    return 0;
}

int removerFilmeDaLista(tipoNoGenero *noGen, char *nomeFilme)
{
    return 0;
}

// realiza a busca pelo nome do filme e retorna o mesmo (APENAS NO GENERO EM QUESTÃO)
tipoNoFilme *buscarFilmeNaLista(tipoNoGenero *noGen, char *nomeFilme)
{
    return 0;
}

void exibirFilmesGenero(tipoNoGenero *noGen)
{
}

// ----------------------------------------------------
// -----------Funções p/ filme assistido---------------
// ----------------------------------------------------

// precisa mesmo comentar?
void inicializarListaAssistidos(tipoListaAssistidos *lista)
{
}

// precisa mesmo comentar?
int inserirAssistido(tipoListaAssistidos *lista, FilmeAssistido filmeAssistido) 
{
    return 0;
}

// já desisti até de colocar o rpecisa msm comentar
int removerAssistido(tipoListaAssistidos *lista, char *nomeFilme) 
{
    return 0;
}

// tipo de retorno att ver outras funções identicas preguiça de comentar ja
tipoNoAssistido* buscarAssistido(tipoListaAssistidos *lista, char *nomeFilme) 
{
    return NULL;
}

void exibirAssistidos(tipoListaAssistidos *lista) 
{
}
// -----------------------------------------------------------------------------------------
// -------------------busca filme e carregamento de arquivo---------------------------------
// -----------------------------------------------------------------------------------------

// procura um filme em todos os gêneros e retorna o primeiro encontrado!!!
Filme* buscarFilmeGlobal(tipoListaGenero *lista, char *nomeFilme) 
{
    return NULL;
}

// lidar com arquivo é fd dependendo pqp mas n é tão dificil tb
void carregarGenerosDoArquivo(tipoListaGenero *lg) 
{
}

void carregarFilmesDoArquivo(tipoListaGenero *lg) 
{
}

// ----------------------------------------------------
// ------------------------mainha----------------------
// ----------------------------------------------------
int main()
{
    int op;
    // uau inicializa as listas uau
    tipoListaGenero listaGeneros;
    inicializarListaGenero(&listaGeneros);
    
    tipoListaAssistidos listaAssistidos;
    inicializarListaAssistidos(&listaAssistidos);
    
    // É MELHOR COMEÇAR INICIALIZANDO ANTES, OU SE A PESSOA QUISER PODE CARREGAR OS DADOS VAZIO E ESCOLHER SE QUER UMA BASE DE DADOS?
    carregarGenerosDoArquivo(&listaGeneros);
    carregarFilmesDoArquivo(&listaGeneros);

    printf("Bem-vindo ao UFSFlix!\n");
    
    do {
        printf("\n----- Menu -----\n");
        printf("1 - Inserir Genero\n");
        printf("2 - Remover Genero\n");
        printf("3 - Exibir Generos\n");
        printf("4 - Exibir Filmes por Genero\n");
        printf("5 - Procurar Genero\n");
        printf("6 - Inserir Filme (Cadastrar Filme)\n");
        printf("7 - Remover Filme\n");
        printf("8 - Exibir Filme Especifico (por genero)\n");
        printf("9 - Procurar Filme Global (por nome)\n");
        printf("10 - Adicionar Filme Assistido\n");
        printf("11 - Remover Filme Assistido\n");
        printf("12 - Exibir Filmes Assistidos\n");
        printf("13 - Pedir Recomendacao de Filme\n");
        printf("0 - Sair do Programa\n");
        printf("Digite sua opcao: ");
        scanf("%d", &op);
        limparBufferEntrada();
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