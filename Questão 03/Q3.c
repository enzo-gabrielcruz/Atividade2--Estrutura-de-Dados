#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define LINE_MAX_SIZE 3001 // máximo de caracteres presentes em uma linha
//========================================================
typedef struct node_duplo {
    int valor;
    struct node_duplo * proximo;
    struct node_duplo * anterior;
}node_duplo;
//========================================================
typedef struct node_circular {
    float num;
    struct node_circular * inicio;
    struct node_circular * fim;
}node_circular;
//========================================================
typedef struct lista_unida {
    struct node_duplo * node_duplo;
    struct node_circular * node_circular;
}lista_unida;
//========================================================

void Iniciar_ListaDupla (node_duplo ** ListaDupla){
    *ListaDupla = NULL;
}

void Iniciar_ListaCircular (node_circular ** ListaCircular){
    *ListaCircular = NULL;
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

            node_duplo * ListaDupla = NULL;
            node_circular * ListaCircular = NULL;

            char *token = strtok(line, " \n");// quebra a linha em pedaços

            while(token){
                if (strcmp(token, "LE") == 0){
                    ListaDupla = malloc(sizeof(ListaDupla));
                    Iniciar_ListaDupla(&ListaDupla);
                }
                if (strcmp(token, "LI") == 0){
                    ListaCircular = malloc(sizeof(&ListaCircular));
                    Iniciar_ListaCircular(&ListaCircular);
                }
                        
                // Utilizar a função isdigit para verificar se o caractere do token é um inteiro ou um float, p/ prosseguir com incremento

                 token = strtok(NULL, " \n");
            }

            // espaço reservado para função de imprimir resultado e também liberação de memória
        }


    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L1Q3.out");

}