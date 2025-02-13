#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 50
#define MAX_DESC 220
#define MAX_SINOPSE 250
#define MAX_COMMENT 200

// struct filme
typedef struct filme {
    char nome[MAX_NOME];
    int duracao;
    int anoLancamento;
    char sinopse[MAX_SINOPSE];
    int avaliacao; // 0 a 5 estrelas?????? tratar isso dps
} Filme;

// nó da lista circ
typedef struct noFilme {
    Filme dado;
    struct noFilme *proxNo;
} tipoNoFilme;

// lista circular
typedef struct listaFilme {
    tipoNoFilme *fim; // ponteiro para o último nó (obviamente fim->prox é o primeiro)
    int quant;
} tipoListaFilme;

// struc do genero
typedef struct genero {
    char nome[MAX_NOME];
    int qntdFilmes;
    char descricao[MAX_DESC];
} Genero;

// nó da lista duplamente encadeada
typedef struct noGenero {
    Genero dado;
    struct noGenero *proxNo;
    struct noGenero *antNo; // excluir????? tratar isso dps
    struct noFilme *proxNoFilme;
} tipoNoGenero;

// lista dup encadeada
typedef struct listaGenero {
    tipoNoGenero *inicio;
    tipoNoGenero *fim;
    int quant;
} tipoListaGenero;

// filme assistido - EXTRA
typedef struct filmeAssistido {
    Filme dado;
    int avaliacaoPessoal;
    char comentario[MAX_COMMENT];
    int favorito; // 0 = Não, 1 = Sim
} FilmeAssistido;

// nó p/ lista de filmes assistidos
typedef struct noAssistido {
    FilmeAssistido dado;
    struct noAssistido *prox;
} tipoNoAssistido;

// lista de filmes assistidos
typedef struct listaAssistidos {
    tipoNoAssistido *inicio;
    int quant;
} tipoListaAssistidos;

// ----------------------------------------------------------
// -----------Funções para o tratamento de string------------
// ----------------------------------------------------------

// utilizar antes de receber a string, obviamente serve para limpar possiveis \n antes da leitura de uma string
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte a string p/ maiuscula
void padronizarString(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Remove o \n de uma string lida pelo fgets
void removeQuebraLinha(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

// -------------------------------------------
// -----------Funções para o Gênero-----------
// -------------------------------------------

// precisa comentar?
void inicializarListaGenero(tipoListaGenero *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quant = 0;
}

// busca um gênero pelo nome (COMPARANDO C/ AS STRING PADRONIZADAS)
tipoNoGenero* buscarGenero(tipoListaGenero *lista, char *nomeGenero) {
    tipoNoGenero *atual = lista->inicio;
    // p/ buscar, converte a string para maiúscula
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

// insere um novo gênero na lista (ordem alfabética)
// ! INSERIR TESTE PARA CHECAR SE AQUELE GENERO JÁ EXISTE NA LISTA (NÃO PODE TER DOIS GENEROS COM O MESMO NOME) USAR BUSCA
int inserirGenero(tipoListaGenero *lista, Genero dado) {
       // Verifica se o gênero já existe na lista
       //conferir rafael*
    if (buscarGenero(lista, dado.nome) != NULL) 
    {
        printf("Erro: O genero '%s' ja existe na lista.\n", dado.nome);
        return 0; // Retorna 0 para indicar que a inserção não foi realizada
    }



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



// remove um gen da lista (libera também a lista de filmes)
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


// precisa comentar?
//exibe os de gÊneros da lista
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
//PRECISA! inicializa a lista de filmes.
void inicializarListaFilme(tipoListaFilme *lista) {
    lista->fim = NULL;
    lista->quant = 0;
}
// busscaa pelo nome
tipoNoFilme* buscarFilmeNaLista(tipoNoGenero *noGen, char *nomeFilme) {
    if (noGen->proxNoFilme == NULL) return NULL;
    tipoNoFilme *atual = noGen->proxNoFilme->proxNo;
    int count = noGen->dado.qntdFilmes;
    do {
        if (strcmp(atual->dado.nome, nomeFilme) == 0)
            return atual;
        atual = atual->proxNo;
        count--;
    } while(count > 0);
    return NULL;
}
// insere em ordem alfabetica
// ! INSERIR TESTE PARA CHECAR SE AQUELE FILME JÁ EXISTE NA LISTA (NÃO PODE TER DOIS FILMES COM O MESMO NOME NA MESMA LISTA DO GENERO) USAR BUSCA
int inserirFilmeNaLista(tipoNoGenero *noGen, Filme filme) {
//conferir* rafael
    if (buscarFilmeNaLista(noGen, filme.nome) != NULL) {
        printf("Erro: O filme '%s' ja existe na lista do genero '%s'.\n", filme.nome, noGen->dado.nome);
        return 0; // Retorna 0 para indicar que a inserção não foi realizada
    }
    tipoNoFilme *novoNo = (tipoNoFilme*) malloc(sizeof(tipoNoFilme));
    if (!novoNo) return 0;
    novoNo->dado = filme;

    if (noGen->proxNoFilme == NULL) {
        novoNo->proxNo = novoNo; // lista vazia aponta para ele mesmo e n pro genero
        noGen->proxNoFilme = novoNo;
    } else {
        // insere em ordem alfabetica
        tipoNoFilme *atual = noGen->proxNoFilme->proxNo; 
        tipoNoFilme *anterior = noGen->proxNoFilme;
        int inserido = 0;
        do {
            if (strcmp(novoNo->dado.nome, atual->dado.nome) < 0) { // uso do strcmp pra comparar
                novoNo->proxNo = atual;
                anterior->proxNo = novoNo;
                inserido = 1;
                break;
            }
            anterior = atual;
            atual = atual->proxNo;
        } while (atual != noGen->proxNoFilme->proxNo);

        if (!inserido) {
            // inserindo no final!!!!!
            novoNo->proxNo = noGen->proxNoFilme->proxNo;
            noGen->proxNoFilme->proxNo = novoNo;
            noGen->proxNoFilme = novoNo;
        }
    }
    noGen->dado.qntdFilmes++;
    return 1;
}




// precisa? remove pelo nome
int removerFilmeDaLista(tipoNoGenero *noGen, char *nomeFilme) {
    if (noGen->proxNoFilme == NULL) return 0; 
    tipoNoFilme *atual = noGen->proxNoFilme->proxNo;
    tipoNoFilme *anterior = noGen->proxNoFilme;
    int encontrado = 0;
    int count = noGen->dado.qntdFilmes;
    do {
        if (strcmp(atual->dado.nome, nomeFilme) == 0) {
            encontrado = 1;
            break;
        }
        anterior = atual;
        atual = atual->proxNo;
        count--;
    } while (count > 0);

    if (!encontrado) return 0;
    if (noGen->dado.qntdFilmes == 1) {
        free(atual);
        noGen->proxNoFilme = NULL;
    } else {
        anterior->proxNo = atual->proxNo;
        if (atual == noGen->proxNoFilme)
            noGen->proxNoFilme = anterior;
        free(atual);
    }
    noGen->dado.qntdFilmes--;
    return 1;
}


// ...
void exibirFilmesGenero(tipoNoGenero *noGen) {
    if (noGen->proxNoFilme == NULL) {
        printf("Nao ha filmes cadastrados para este genero.\n");
        return;
    }
    tipoNoFilme *atual = noGen->proxNoFilme->proxNo;
    int count = noGen->dado.qntdFilmes;
    do {
        printf("Nome: %s\n", atual->dado.nome);
        printf("Ano: %d\n", atual->dado.anoLancamento);
        printf("Duracao: %d min\n", atual->dado.duracao);
        printf("Sinopse: %s\n", atual->dado.sinopse);
        printf("Avaliacao: %d\n", atual->dado.avaliacao);
        printf("---------------------\n");
        atual = atual->proxNo;
        count--;
    } while (count > 0);
}

// ----------------------------------------------------
// -----------Funções p/ filme assistido---------------
// ----------------------------------------------------

// precisa mesmo comentar?
void inicializarListaAssistidos(tipoListaAssistidos *lista) {
    lista->inicio = NULL;
    lista->quant = 0;
}

// precisa mesmo comentar?
int inserirAssistido(tipoListaAssistidos *lista, FilmeAssistido filmeAssistido) {
    tipoNoAssistido *novo = (tipoNoAssistido*) malloc(sizeof(tipoNoAssistido));
    if (!novo) return 0;
    novo->dado = filmeAssistido;
    novo->prox = lista->inicio;
    lista->inicio = novo;
    lista->quant++;
    return 1;
}

// já desisti até de colocar o rpecisa msm comentar, na vdd quase
int removerAssistido(tipoListaAssistidos *lista, char *nomeFilme) {
    tipoNoAssistido *atual = lista->inicio;
    tipoNoAssistido *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dado.dado.nome, nomeFilme) == 0) {
            if (anterior == NULL)
                lista->inicio = atual->prox;
            else
                anterior->prox = atual->prox;
            free(atual);
            lista->quant--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

// ...
tipoNoAssistido* buscarAssistido(tipoListaAssistidos *lista, char *nomeFilme) {
    tipoNoAssistido *atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dado.dado.nome, nomeFilme) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}


void exibirAssistidos(tipoListaAssistidos *lista) {
    tipoNoAssistido *atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhum filme assistido cadastrado.\n");
        return;
    }
    while (atual != NULL) {
        printf("Nome: %s\n", atual->dado.dado.nome);
        printf("Ano: %d\n", atual->dado.dado.anoLancamento);
        printf("Duracao: %d min\n", atual->dado.dado.duracao);
        printf("Sinopse: %s\n", atual->dado.dado.sinopse);
        printf("Avaliacao Pessoal: %d\n", atual->dado.avaliacaoPessoal);
        printf("Comentario: %s\n", atual->dado.comentario);
        printf("Favorito: %s\n", atual->dado.favorito ? "Sim" : "Nao");
        printf("-----------------------\n");
        atual = atual->prox;
    }
}
// -----------------------------------------------------------------------------------------
// -------------------busca filme e carregamento de arquivo---------------------------------
// -----------------------------------------------------------------------------------------

// !!! (procura em todos os gêneros e retorna o primeiro encontrado!!!!) (se vc tiver o error lens no vs esse comentario faz mais sentido)
Filme* buscarFilmeGlobal(tipoListaGenero *lista, char *nomeFilme) {
    tipoNoGenero *atualGen = lista->inicio;
    while (atualGen != NULL) {
        if (atualGen->dado.qntdFilmes > 0) {
            tipoNoFilme *atualFilme = atualGen->proxNoFilme->proxNo;
            int count = atualGen->dado.qntdFilmes;
            do {
                if (strcmp(atualFilme->dado.nome, nomeFilme) == 0)
                    return &(atualFilme->dado);
                atualFilme = atualFilme->proxNo;
                count--;
            } while(count > 0);
        }
        atualGen = atualGen->proxNo;
    }
    return NULL;
}

// chato
void carregarGenerosDoArquivo(tipoListaGenero *lg) {
    FILE *arq = fopen("c:/Users/04rfl/atividadeED/priv_atvd/generos.txt", "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo generos.txt\n");
        return;
    }
    char linha[256];
    while (fgets(linha, sizeof(linha), arq)) {
        // Remove a quebra de linha
        linha[strcspn(linha, "\n")] = 0;
        // Formato esperado: Nome;Descricao
        char *nome = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        if (nome && descricao) {
            Genero g;
            strncpy(g.nome, nome, MAX_NOME);
            strncpy(g.descricao, descricao, MAX_DESC);
            g.qntdFilmes = 0;
            inserirGenero(lg, g);
        }
    }
    fclose(arq);
}

// chato
void carregarFilmesDoArquivo(tipoListaGenero *lg) {
    FILE *arq = fopen("c:/Users/04rfl/atividadeED/priv_atvd/filmes.txt", "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo filmes.txt\n");
        return;
    }
    char linha[256];
    while (fgets(linha, sizeof(linha), arq)) {
        linha[strcspn(linha, "\n")] = 0;
        // Formato esperado: Genero;NomeFilme;Ano;Duracao;Sinopse;Avaliacao
        char *generoNome   = strtok(linha, ";");
        char *filmeNome    = strtok(NULL, ";");
        char *anoStr       = strtok(NULL, ";");
        char *duracaoStr   = strtok(NULL, ";");
        char *sinopse      = strtok(NULL, ";");
        char *avaliacaoStr = strtok(NULL, ";");
        if (generoNome && filmeNome && anoStr && duracaoStr && sinopse && avaliacaoStr) {
            tipoNoGenero *ng = buscarGenero(lg, generoNome);
            if (ng) {
                Filme f;
                strncpy(f.nome, filmeNome, MAX_NOME);
                f.anoLancamento = atoi(anoStr);
                f.duracao = atoi(duracaoStr);
                strncpy(f.sinopse, sinopse, MAX_SINOPSE);
                f.avaliacao = atoi(avaliacaoStr);
                inserirFilmeNaLista(ng, f);
                ng->dado.qntdFilmes++;
            }
        }
    }
    fclose(arq);
}

// ----------------------------------------------------
// ------------------------mainha----------------------
// ----------------------------------------------------

int main() {
    int op;
    
    // uau inicializa as listas uau
    tipoListaGenero listaGeneros;
    inicializarListaGenero(&listaGeneros);
    
    tipoListaAssistidos listaAssistidos;
    inicializarListaAssistidos(&listaAssistidos);

    // É MELHOR COMEÇAR INICIALIZANDO ANTES, OU SE A PESSOA QUISER PODE CARREGAR OS DADOS VAZIO E ESCOLHER SE QUER UMA BASE DE DADOS?
    carregarGenerosDoArquivo(&listaGeneros);
    carregarFilmesDoArquivo(&listaGeneros);

    printf("Base de dados pre-carregada:\n");
    exibirGeneros(&listaGeneros);
    
    printf("Bem-vindo ao UFSFlix!\n");
    
    do {
        printf("\n----- Menu -----\n");
        printf("1 - Inserir Genero\n");
        printf("2 - Remover Genero\n");
        printf("3 - Exibir Generos\n");
        printf("4 - Exibir Filmes por Genero\n");
        printf("5 - Procurar Genero\n");
        printf("6 - Inserir Filme (Cadastrar Filme)\n");
        printf("7 - Remover Filme de um Genero\n");
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
        
        switch(op) {
            case 1: {
                // Inserir Genero
                Genero novoGenero;
                printf("Digite o nome do genero: ");
                fgets(novoGenero.nome, MAX_NOME, stdin);
                removeQuebraLinha(novoGenero.nome);
                
                printf("Digite uma descricao para o genero: ");
                fgets(novoGenero.descricao, MAX_DESC, stdin);
                removeQuebraLinha(novoGenero.descricao);
                novoGenero.qntdFilmes = 0;
                
                if (inserirGenero(&listaGeneros, novoGenero))
                    printf("Genero inserido com sucesso.\n");
                else
                    printf("Erro ao inserir genero.\n");
                break;
            }
            case 2: {
                char nomeGenero[MAX_NOME];
                printf("Digite o nome do genero a remover: ");
                fgets(nomeGenero, MAX_NOME, stdin);
                removeQuebraLinha(nomeGenero);
                if(removerGenero(&listaGeneros, nomeGenero))
                    printf("Genero removido com sucesso.\n");
                else
                    printf("Genero nao encontrado.\n");
                break;
            }
            case 3: {
                exibirGeneros(&listaGeneros);
                break;
            }
            case 4: {
                char nomeGenero[MAX_NOME];
                printf("Digite o nome do genero: ");
                fgets(nomeGenero, MAX_NOME, stdin);
                removeQuebraLinha(nomeGenero);
                tipoNoGenero *noGen = buscarGenero(&listaGeneros, nomeGenero);
                if(noGen) {
                    printf("Filmes do genero %s:\n", noGen->dado.nome);
                    exibirFilmesGenero(noGen);
                } else {
                    printf("Genero nao encontrado.\n");
                }
                break;
            }
            case 5: {
                char nomeGenero[MAX_NOME];
                printf("Digite o nome do genero para pesquisa: ");
                fgets(nomeGenero, MAX_NOME, stdin);
                removeQuebraLinha(nomeGenero);
                tipoNoGenero *noGen = buscarGenero(&listaGeneros, nomeGenero);
                if(noGen) {
                    printf("Genero: %s\n", noGen->dado.nome);
                    printf("Descricao: %s\n", noGen->dado.descricao);
                    printf("Quantidade de filmes: %d\n", noGen->dado.qntdFilmes);
                } else {
                    printf("Genero nao encontrado.\n");
                }
                break;
            }
            case 6: {
                Filme novoFilme;
                int numGeneros;
                printf("Digite o nome do filme: ");
                fgets(novoFilme.nome, MAX_NOME, stdin);
                removeQuebraLinha(novoFilme.nome);
                printf("Digite a duracao (minutos): ");
                scanf("%d", &novoFilme.duracao);
                limparBufferEntrada();
                printf("Digite o ano de lancamento: ");
                scanf("%d", &novoFilme.anoLancamento);
                limparBufferEntrada();
                printf("Digite a sinopse do filme: ");
                fgets(novoFilme.sinopse, MAX_SINOPSE, stdin);
                removeQuebraLinha(novoFilme.sinopse);
                printf("Digite a avaliacao (0 a 5): ");
                scanf("%d", &novoFilme.avaliacao);
                limparBufferEntrada();
                printf("Quantos generos este filme pertence? ");
                // ! ADD VERIFICADOR SE O NUM DIGITADO É UM INTEIRO
                scanf("%d", &numGeneros);
                limparBufferEntrada();
                
                for (int i = 0; i < numGeneros; i++) {
                    char nomeGenero[MAX_NOME];
                    printf("Digite o nome do genero (%d): ", i+1);
                    fgets(nomeGenero, MAX_NOME, stdin);
                    removeQuebraLinha(nomeGenero);
                    tipoNoGenero *noGen = buscarGenero(&listaGeneros, nomeGenero);
                    if(noGen) {
                        if(inserirFilmeNaLista(noGen, novoFilme)) {
                            noGen->dado.qntdFilmes++;
                            printf("Filme inserido no genero %s.\n", noGen->dado.nome);
                        } else {
                            printf("Erro ao inserir filme no genero %s.\n", noGen->dado.nome);
                        }
                    } else {
                        printf("Genero %s nao encontrado. Filme nao inserido neste genero.\n", nomeGenero);
                    }
                }
                break;
            }
            case 7: {
                char nomeGenero[MAX_NOME], nomeFilme[MAX_NOME];
                printf("Digite o nome do genero: ");
                fgets(nomeGenero, MAX_NOME, stdin);
                removeQuebraLinha(nomeGenero);
                tipoNoGenero *noGen = buscarGenero(&listaGeneros, nomeGenero);
                if(noGen) {
                    printf("Digite o nome do filme a remover: ");
                    fgets(nomeFilme, MAX_NOME, stdin);
                    removeQuebraLinha(nomeFilme);
                    if(removerFilmeDaLista(noGen, nomeFilme)) {
                        noGen->dado.qntdFilmes--;
                        printf("Filme removido com sucesso.\n");
                    } else {
                        printf("Filme nao encontrado neste genero.\n");
                    }
                } else {
                    printf("Genero nao encontrado.\n");
                }
                break;
            }
            case 8: {
                char nomeGenero[MAX_NOME], nomeFilme[MAX_NOME];
                printf("Digite o nome do genero: ");
                fgets(nomeGenero, MAX_NOME, stdin);
                removeQuebraLinha(nomeGenero);
                tipoNoGenero *noGen = buscarGenero(&listaGeneros, nomeGenero);
                if(noGen) {
                    printf("Digite o nome do filme: ");
                    fgets(nomeFilme, MAX_NOME, stdin);
                    removeQuebraLinha(nomeFilme);
                    tipoNoFilme *filmeEncontrado = buscarFilmeNaLista(noGen, nomeFilme);
                    if(filmeEncontrado) {
                        printf("Detalhes do filme:\n");
                        printf("Nome: %s\n", filmeEncontrado->dado.nome);
                        printf("Ano: %d\n", filmeEncontrado->dado.anoLancamento);
                        printf("Duracao: %d min\n", filmeEncontrado->dado.duracao);
                        printf("Sinopse: %s\n", filmeEncontrado->dado.sinopse);
                        printf("Avaliacao: %d\n", filmeEncontrado->dado.avaliacao);
                    } else {
                        printf("Filme nao encontrado neste genero.\n");
                    }
                } else {
                    printf("Genero nao encontrado.\n");
                }
                break;
            }
            case 9: {
                char nomeFilme[MAX_NOME];
                printf("Digite o nome do filme para pesquisa: ");
                fgets(nomeFilme, MAX_NOME, stdin);
                removeQuebraLinha(nomeFilme);
                Filme *filmePtr = buscarFilmeGlobal(&listaGeneros, nomeFilme);
                if(filmePtr) {
                    printf("Filme encontrado:\n");
                    printf("Nome: %s\n", filmePtr->nome);
                    printf("Ano: %d\n", filmePtr->anoLancamento);
                    printf("Duracao: %d min\n", filmePtr->duracao);
                    printf("Sinopse: %s\n", filmePtr->sinopse);
                    printf("Avaliacao: %d\n", filmePtr->avaliacao);
                } else {
                    printf("Filme nao encontrado.\n");
                }
                break;
            }
            case 10: {
                char nomeFilme[MAX_NOME];
                printf("Digite o nome do filme assistido: ");
                fgets(nomeFilme, MAX_NOME, stdin);
                removeQuebraLinha(nomeFilme);
                Filme *filmePtr = buscarFilmeGlobal(&listaGeneros, nomeFilme);
                if(filmePtr) {
                    FilmeAssistido novoAssistido;
                    novoAssistido.dado = *filmePtr; // copia os dados do filme
                    printf("Digite sua avaliacao pessoal (0 a 5): ");
                    scanf("%d", &novoAssistido.avaliacaoPessoal);
                    limparBufferEntrada();
                    printf("Digite um comentario sobre o filme: ");
                    fgets(novoAssistido.comentario, MAX_COMMENT, stdin);
                    removeQuebraLinha(novoAssistido.comentario);
                    printf("Marcar como favorito? (1-Sim, 0-Nao): ");
                    scanf("%d", &novoAssistido.favorito);
                    limparBufferEntrada();
                    if(inserirAssistido(&listaAssistidos, novoAssistido))
                        printf("Filme assistido adicionado.\n");
                    else
                        printf("Erro ao adicionar filme assistido.\n");
                } else {
                    printf("Filme nao encontrado nos cadastros.\n");
                }
                break;
            }
            case 11: {
                char nomeFilme[MAX_NOME];
                printf("Digite o nome do filme assistido a remover: ");
                fgets(nomeFilme, MAX_NOME, stdin);
                removeQuebraLinha(nomeFilme);
                if(removerAssistido(&listaAssistidos, nomeFilme))
                    printf("Filme assistido removido.\n");
                else
                    printf("Filme assistido nao encontrado.\n");
                break;
            }
            case 12: {
                exibirAssistidos(&listaAssistidos);
                break;
            }
            case 0: {
                printf("Encerrando o programa.\n");
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
        
    } while(op != 0);
    
    return 0;
}
