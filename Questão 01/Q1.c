// Equipe: Enzo Gabriel Calado e João Neto 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define LINE_MAX_SIZE 3001 // máximo de caracteres presentes em uma linha

typedef struct node{
    int valor;
    struct node *proximo;
}node;

typedef struct lista{
    struct node * inicio;
}lista;

typedef struct guarda_lista{
    struct node * inicio;
    int soma;
}guarda_lista;

void iniciar_lista (lista * lista){
   lista->inicio = NULL;
}

void preencher_lista (char *token, lista * lista){
    node *new = malloc (sizeof(node)); // alocando memória para guardar o token em uma posicao da lista
    node *aux;
    int num = atoi(token); // converte o token (pedaço da string) em inteiro
    new->valor = num; 

    // OBSERVAÇÃO - LISTA JÁ SERÁ PREENCHIDA DE FORMA ORDENADA
    if (lista->inicio == NULL){
        new->proximo = NULL;
        lista->inicio = new;
    }
    else if (new->valor < lista->inicio->valor){
        new->proximo = lista->inicio;
        lista->inicio = new;
    }
    else{
        aux = lista->inicio;
        while (aux->proximo != NULL && aux->proximo->valor <= new->valor)
            aux = aux->proximo; // percorrer o laço até encontrar a posição ordenda para posicionar o new->valor
            new->proximo = aux->proximo;
            aux->proximo = new;
        
    }
}

int tamanho_lista(lista *lista){
    node *node = lista->inicio;
    int soma = 0;
    while(node != NULL){
        soma = soma + (node->valor);
        node = node->proximo;
    }
    return soma;
}

void armazenar_lista (guarda_lista ** vetor, lista *lista, int *qtd){
    //função responsável por armazenar todas as listas presentes em uma linha (necessário alocar memória);
    // armazenaremos a lista de forma ordenada da maior para menor (DECRESCENTE);
    int soma = tamanho_lista(lista);
    guarda_lista *aux = realloc(*vetor,(*qtd + 1) * sizeof(guarda_lista)); // realoca memória a cada  acréscimo de lista na linha
    *vetor = aux;

    //criar cópia da lista
    node *copia_inicio = NULL;
    node *copia_fim = NULL;
    node *atual = lista->inicio;

    while (atual) {
        node *new = malloc(sizeof(node));
        new->valor = atual->valor;
        new->proximo = NULL;
        
        if (copia_inicio == NULL) {
            copia_inicio = new;
            copia_fim = new;
        } else {
            copia_fim->proximo = new;
            copia_fim = new;
        }
        
        atual = atual->proximo;
    }

    (*vetor)[*qtd].inicio = copia_inicio;
    (*vetor)[*qtd].soma = soma;
    (*qtd)++;// incrementa a quantidade de listas já que foi adcionado mais uma lista ao vetor de struct;
}

void imprimir_lista_com_espaco(FILE *out, guarda_lista *guarda_lista, bool ultima_lista) {
    node *atual = guarda_lista->inicio;
    fprintf(out, "start ");
    while (atual != NULL) {
        if (atual->proximo == NULL && ultima_lista)
            fprintf(out, "%d", atual->valor); // Último número da última lista → sem espaço
        else
            fprintf(out, "%d ", atual->valor); // Todos os demais → com espaço
        atual = atual->proximo;
    }
}

void imprimir_lista_final(FILE *out, guarda_lista *vetor, int qtd) {
    // Identificar qual é a última lista que será impressa
    int ultimo_idx = -1;

    for (int i = qtd - 1; i >= 0; i--) {
        bool unica = true;
        for (int j = i + 1; j < qtd; j++) {
            if (vetor[i].soma == vetor[j].soma) {
                unica = false;
                break;
            }
        }
        if (unica) {
            ultimo_idx = i;
            break;
        }
    }

    for (int i = 0; i < qtd; i++) {
        bool unica = true;
        for (int j = i + 1; j < qtd; j++) {
            if (vetor[i].soma == vetor[j].soma) {
                unica = false;
                break;
            }
        }

        if (unica) {
            bool ultima = (i == ultimo_idx);
            imprimir_lista_com_espaco(out, &vetor[i], ultima);
        }
    }

    fprintf(out, "\n");
}

bool is_inteiro_valido(const char *str) {
    if (*str == '-' || *str == '+') str++;  // permite sinal no início
    if (*str == '\0') return false;         // apenas sinal, sem número

    while (*str) {
        if (!isdigit((unsigned char)*str)) return false;  // tem letra? falso
        str++;
    }
    return true;
}

int comparar_desc(const void *a, const void *b) {
    guarda_lista *listaA = (guarda_lista *)a;
    guarda_lista *listaB = (guarda_lista *)b;
    return listaB->soma - listaA->soma;  // decrescente
}

void liberar_lista(lista * lista) {
    node *atual = lista->inicio;
    while (atual) {
        node *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(lista);
}

void liberar_node (node * inicio){
    node *atual = inicio;
    while (atual) {
        node *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
}

void remover_ultima_linha_em_branco(const char *file_out) { // função utilizada para apagar a última linha em branco
    FILE *arquivo = fopen(file_out, "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para leitura");
        return;
    }

    // Descobre tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    // Lê todo o conteúdo
    char *conteudo = malloc(tamanho + 1);
    if (!conteudo) {
        fclose(arquivo);
        fprintf(stderr, "Erro de alocação\n");
        return;
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';
    fclose(arquivo);

    // Remove último \n se existir
    if (tamanho > 0 && conteudo[tamanho - 1] == '\n') {
        conteudo[tamanho - 1] = '\0';
        tamanho--;
    }

    // Regrava o arquivo sem a última quebra de linha
    arquivo = fopen(file_out, "wb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para escrita");
        free(conteudo);
        return;
    }

    fwrite(conteudo, 1, tamanho, arquivo);
    fclose(arquivo);
    free(conteudo);
}

int main(){
    FILE *file_in = fopen("L1Q1.in", "r");
    FILE *file_out = fopen("L1Q1.out", "w");
        if (!file_in) {
            perror("Erro ao abrir L1Q1.in");
            return 1;
        }
        if (!file_out) {
            perror("Erro ao abrir L1Q1.out");
            return 1;
        }
    char line[LINE_MAX_SIZE]; 

    while(fgets(line,LINE_MAX_SIZE,file_in)){
        //este fgets lê uma linha completa do arquivo in e armazena em um ponteiro e faz isso até não existir mais linhas a serem lidas
            char *apontador = line; // percorre toda a linha, caractere por caractere
            if (*apontador == '\0') continue;  // linha vazia → pula

            char *token = strtok(line, " \n");// quebra a linha em pedaços
            guarda_lista *guarda_lista = NULL; // iniciando o vetor de struct p/ guardar todas as listas presentes em uma linha
            lista *lista = NULL;
            int qtd = 0; // define o tamanho do vetor com várias listas
             
            while (token) {
                if (strcmp(token, "start") == 0) {
                    if (lista != NULL) { // Se já existe uma lista, armazena antes de criar nova
                        if (lista != NULL && lista->inicio != NULL) {
                            armazenar_lista(&guarda_lista, lista, &qtd);
                        }
                        if (lista != NULL)
                            liberar_lista(lista);
                        

                    }
                    lista = malloc(sizeof(*lista));
                    iniciar_lista(lista);
                }
                else if (lista != NULL && is_inteiro_valido(token)) {
                    preencher_lista(token, lista);
                }
                token = strtok(NULL, " \n");
            }

            // armazena a última lista da linha
            if (lista != NULL && lista->inicio != NULL) 
                armazenar_lista(&guarda_lista, lista, &qtd);
            if (lista != NULL)
                liberar_lista(lista);
            qsort(guarda_lista, qtd, sizeof(guarda_lista[0]), comparar_desc);
            imprimir_lista_final(file_out,guarda_lista, qtd);

            for (int i = 0; i < qtd; i++) {
                liberar_node(guarda_lista[i].inicio);
            }
            free(guarda_lista);
    }


    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L1Q1.out");
}