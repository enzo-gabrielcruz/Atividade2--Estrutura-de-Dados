// Equipe: Enzo Gabriel Calado e João Neto 
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define LINE_MAX_SIZE 3001

typedef struct node{
    char pessoa[20];
    struct node * proximo;
}node;

void push (node * topo){ // função empilhar
    node * novo = malloc(sizeof(node));

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
    FILE *file_in = fopen("L1Q2.in", "r");
    FILE *file_out = fopen("L1Q2.out", "w");
    if (!file_in) {
            perror("Erro ao abrir L1Q2.in");
            return 1;
        }
        if (!file_out) {
            perror("Erro ao abrir L1Q2.out");
            return 1;
        }
    char line[LINE_MAX_SIZE]; 

    while(fgets(line,LINE_MAX_SIZE,file_in)){
        char * apontador = line;
        if (*apontador == '\0') continue;  // linha vazia → pula

        char *token = strtok(line, " \n");// quebra a linha em pedaços
        node * topo = NULL; // a pilha sempre colocaremos o proximo elemento no topo

        while(token){

        }

    }

    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L1Q2.out");
}   */

// Equipe: Enzo Gabriel Calado e João Neto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_MAX_SIZE 3001

// Estrutura da pilha
typedef struct node {
    char pessoa[20];
    struct node *proximo;
} node;

// Função que processa uma linha e gera os comandos de push/pop ordenados
void processar_linha(char *linha, FILE *file_out) {
    node *topo = NULL;
    node *temporarios = NULL;

    char *token = strtok(linha, " \n");
    while (token != NULL) {
        int pops = 0;

        // Retira elementos do topo enquanto forem menores que o token atual
        while (topo && strcmp(topo->pessoa, token) < 0) {
            node *temp = topo;
            topo = topo->proximo;

            temp->proximo = temporarios;
            temporarios = temp;
            pops++;
        }

        // Escreve pop agrupado, se houver
        if (pops > 0) {
            fprintf(file_out, "%dx-pop ", pops);
        }

        // Empilha o novo token
        node *novo = malloc(sizeof(node));
        strcpy(novo->pessoa, token);
        novo->proximo = topo;
        topo = novo;
        fprintf(file_out, "push-%s ", token);

        // Reempilha os itens temporários (na ordem original)
        while (temporarios != NULL) {
            node *voltar = temporarios;
            temporarios = temporarios->proximo;

            voltar->proximo = topo;
            topo = voltar;
            fprintf(file_out, "push-%s ", voltar->pessoa);
        }

        token = strtok(NULL, " \n");
    }

    fprintf(file_out, "\n");

    // Libera memória da pilha final
    while (topo) {
        node *tmp = topo;
        topo = topo->proximo;
        free(tmp);
    }
}

// Função que remove a última linha em branco do arquivo de saída
void remover_ultima_linha_em_branco(const char *file_out) {
    FILE *arquivo = fopen(file_out, "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para leitura");
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    char *conteudo = malloc(tamanho + 1);
    if (!conteudo) {
        fclose(arquivo);
        fprintf(stderr, "Erro de alocação\n");
        return;
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';
    fclose(arquivo);

    // Remove última quebra de linha
    if (tamanho > 0 && conteudo[tamanho - 1] == '\n') {
        conteudo[tamanho - 1] = '\0';
        tamanho--;
    }

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

int main() {
    FILE *file_in = fopen("L1Q2.in", "r");
    FILE *file_out = fopen("L1Q2.out", "w");
    if (!file_in) {
        perror("Erro ao abrir L1Q2.in");
        return 1;
    }
    if (!file_out) {
        perror("Erro ao abrir L1Q2.out");
        fclose(file_in);
        return 1;
    }

    char line[LINE_MAX_SIZE];

    // Processa cada linha do arquivo de entrada
    while (fgets(line, LINE_MAX_SIZE, file_in)) {
        if (line[0] == '\n') continue;  // ignora linhas vazias
        processar_linha(line, file_out);
    }

    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L1Q2.out");

    return 0;
}
