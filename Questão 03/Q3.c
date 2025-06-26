#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>


#define LINE_MAX_SIZE 3001 // máximo de caracteres presentes em uma linha
//========================================================
typedef struct node_duplo {
    int valor;
    struct node_duplo * proximo;
    struct node_duplo * anterior;
}node_duplo;
//========================================================
typedef struct node_circular {
    float valor_float;
    struct node_circular * proximo;
    int usado;
}node_circular;

typedef struct lista_circular {
    node_circular * inicio;
    node_circular * fim;
}lista_circular;
//========================================================
typedef struct lista_unida {
    struct node_duplo * node_duplo;
    struct node_circular * node_circular;
}lista_unida;
//========================================================

void Iniciar_ListaDupla (node_duplo ** ListaDupla){
    *ListaDupla = NULL;
}

void Iniciar_ListaCircular (lista_circular * ListaCircular){
    ListaCircular->inicio = NULL;
    ListaCircular->fim = NULL;
}

int eh_inteiro(const char *str) { // verifica os caracteres inteiros
    // Confere se é número inteiro (sem ponto)
    if (*str == '-') str++;  // Suporta negativos
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int eh_float(const char *str) { // verifica os caracteres float
    int ponto = 0;
    if (*str == '-') str++;  // Suporta negativos
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') {
            if (ponto) return 0; // dois pontos, não é float
            ponto = 1;
        } else if (!isdigit(str[i])) return 0;
    }
    return ponto; // só retorna true se tiver ponto
}

void Preencher_ListaDupla (node_duplo ** ListaDupla, char *token){ // lista dupla é preenchida por números inteiros
    node_duplo *novo = malloc(sizeof(node_duplo));
    int num = atoi(token);
    novo->valor = num;
    novo->anterior = NULL;
    novo->proximo = NULL;

    if (*ListaDupla == NULL) {
        *ListaDupla = novo;
        return;
    }

    node_duplo *atual = *ListaDupla;

    // inserção no início
    if (num < atual->valor) {
        novo->proximo = atual;
        atual->anterior = novo;
        *ListaDupla = novo;
        return;
    }

    // busca posição correta
    while (atual->proximo && atual->proximo->valor < num)
        atual = atual->proximo;

    novo->proximo = atual->proximo;
    novo->anterior = atual;

    if (atual->proximo)
        atual->proximo->anterior = novo;

    atual->proximo = novo;
}

void Preencher_ListaCircular (lista_circular * ListaCircular, char *token){ // lista circular é preenchida por floats
    node_circular *novo = malloc(sizeof(node_circular));
    float valor_float = strtof(token, NULL);
    novo->valor_float = valor_float;
    novo->proximo = NULL;

    // lista vazia
    if (ListaCircular->inicio == NULL) {
        novo->proximo = novo;
        novo->usado = 0; // // //
        ListaCircular->inicio = ListaCircular->fim = novo;
        return;
    }

    node_circular *atual = ListaCircular->inicio;
    node_circular *anterior = ListaCircular->fim;

    do {
        if (valor_float > atual->valor_float) break;
        anterior = atual;
        atual = atual->proximo;
    } while (atual != ListaCircular->inicio);

    novo->proximo = atual;
    anterior->proximo = novo;

    // inserção no início → precisa atualizar inicio
    if (valor_float > ListaCircular->inicio->valor_float)
        ListaCircular->inicio = novo;

    // inserção após o fim → precisa atualizar fim
    if (anterior == ListaCircular->fim && valor_float <= ListaCircular->fim->valor_float)
        ListaCircular->fim = novo;
}

void Liberar_ListaCircular(lista_circular *lista) {
    if (!lista || !lista->inicio) return;

    node_circular *inicio = lista->inicio;
    node_circular *atual = inicio->proximo;

    while (atual != inicio) {
        node_circular *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(inicio);
    free(lista);
}

void Liberar_ListaDupla(node_duplo *lista) {
    while (lista) {
        node_duplo *atual = lista;
        lista = lista->proximo;
        free(atual);
    }
}

void printer(FILE *out, node_duplo *le, lista_circular *li) {
    fprintf(out, "[");  // Início da linha

    node_circular *li_atual;

    while (le != NULL) {
        fprintf(out, "%d(", le->valor);

        li_atual = li->inicio;
        int primeiro = 1;

        if (li_atual) {
            do {
                float diff = fabs(le->valor - li_atual->valor_float);
                if (diff < 1.0 && !li_atual->usado) {
                    if (!primeiro) fprintf(out, "->");
                    fprintf(out, "%.2f", li_atual->valor_float);
                    primeiro = 0;
                    li_atual->usado = 1;
                }
                li_atual = li_atual->proximo;
            } while (li_atual != li->inicio);
        }

        fprintf(out, ")");

        if (le->proximo != NULL)
            fprintf(out, "->");

        le = le->proximo;
    }

    fprintf(out, "]\n");  // Fim da linha
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
    FILE *file_in = fopen("L1Q3.in", "r");
    FILE *file_out = fopen("L1Q3.out", "w");
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

            node_duplo * ListaDupla = NULL;
            lista_circular * ListaCircular;

            char *token = strtok(line, " \n");// quebra a linha em pedaços

            while(token){
                if (strcmp(token, "LE") == 0){
                    Iniciar_ListaDupla(&ListaDupla);
                }
                else if (strcmp(token, "LI") == 0){
                    ListaCircular = malloc(sizeof(ListaCircular));
                    Iniciar_ListaCircular(ListaCircular);
                }
                else if (eh_float(token))
                    Preencher_ListaCircular(ListaCircular,token);
                else if (eh_inteiro(token))
                    Preencher_ListaDupla(&ListaDupla,token);
                 token = strtok(NULL, " \n");
            }

            // Reinicia os usados da LI
            if (ListaCircular && ListaCircular->inicio) {
                node_circular *atual = ListaCircular->inicio;
                do {
                    atual->usado = 0;
                    atual = atual->proximo;
                } while (atual != ListaCircular->inicio);
            }
            // espaço reservado para função de imprimir resultado e também liberação de memória
            printer(file_out,ListaDupla,ListaCircular);
            Liberar_ListaDupla(ListaDupla);
            Liberar_ListaCircular(ListaCircular);
        }


    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L1Q3.out");

}