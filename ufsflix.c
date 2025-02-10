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

// Insere um novo gênero na lista (ordem alfabética baseada em nome)
int inserirGenero(tipoListaGenero *lista, Genero dado) {
    tipoNoGenero *novoNo = (tipoNoGenero*) malloc(sizeof(tipoNoGenero));
    if (!novoNo) return 0;
    novoNo->dado = dado;
    novoNo->proxNoFilme = NULL;
    novoNo->proxNo = NULL;
    novoNo->antNo = NULL;

    // Se a lista estiver vazia
    if (lista->quant == 0) {
        lista->inicio = novoNo;
        lista->fim = novoNo;
    } else {
        tipoNoGenero *atual = lista->inicio;
        tipoNoGenero *anterior = NULL;
        while (atual != NULL && strcmp(atual->dado.nome, novoNo->dado.nome) < 0) {
            anterior = atual;
            atual = atual->proxNo;
        }
        if (anterior == NULL) { // Inserir no início
            novoNo->proxNo = lista->inicio;
            lista->inicio->antNo = novoNo;
            lista->inicio = novoNo;
        } else {
            novoNo->proxNo = atual;
            novoNo->antNo = anterior;
            anterior->proxNo = novoNo;
            if (atual != NULL) {
                atual->antNo = novoNo;
            } else {
                lista->fim = novoNo;
            }
        }
    }
    lista->quant++;
    return 1;
}

// Busca um gênero pelo nome (comparação considerando strings padronizadas)
tipoNoGenero* buscarGenero(tipoListaGenero *lista, char *nomeGenero) {
    tipoNoGenero *atual = lista->inicio;
    // Para busca, convertemos a string para maiúsculas
    char nomeBusca[MAX_NOME];
    strcpy(nomeBusca, nomeGenero);
    padronizarString(nomeBusca);
    while (atual != NULL) {
        char nomeGen[MAX_NOME];
        strcpy(nomeGen, atual->dado.nome);
        padronizarString(nomeGen);
        if (strcmp(nomeGen, nomeBusca) == 0)
            return atual;
        atual = atual->proxNo;
    }
    return NULL;
}

// Remove um gênero da lista (libera também a lista de filmes associada)
int removerGenero(tipoListaGenero *lista, char *nomeGenero) {
    tipoNoGenero *atual = lista->inicio;
    // procura o gênero na lista
    while (atual != NULL) {
        if (strcmp(atual->dado.nome, nomeGenero) == 0) {
            if (atual->antNo == NULL) {
                lista->inicio = atual->proxNo;
                if (lista->inicio != NULL) {
                    lista->inicio->antNo = NULL;
                }
            } else {
                atual->antNo->proxNo = atual->proxNo;
                if (atual->proxNo != NULL) {
                    atual->proxNo->antNo = atual->antNo;
                }
            }
            if (atual == lista->fim) {
                lista->fim = atual->antNo;
            }
            // serve p liberar a lista de filmes
            if (atual->proxNoFilme) {
                tipoNoFilme *filmeAtual = atual->proxNoFilme->proxNo;
                tipoNoFilme *temp;
                int count = atual->dado.qntdFilmes;
                while (count--)  { // vai diminuindo a qnt de de filmes liberando os nós da lista de filmes
                    temp = filmeAtual;
                    filmeAtual = filmeAtual->proxNo;
                    free(temp);
                }
            }
            free(atual);
            lista->quant--;
            return 1;
        }
        atual = atual->proxNo;
    }
    return 0;
}

// Exibe todos os gêneros cadastrados
void exibirGeneros(tipoListaGenero *lista) {
    tipoNoGenero *atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhum genero cadastrado.\n");
        return;
    }
    while (atual != NULL) {
        printf("Genero: %s\n", atual->dado.nome);
        printf("Descricao: %s\n", atual->dado.descricao);
        printf("Quantidade de filmes: %d\n", atual->dado.qntdFilmes);
        printf("---------------------------\n");
        atual = atual->proxNo;
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