#include <string.h>
#include "funcs_uteis.h"    //Já incluso o <stdio.h>

void imprime_traco(void){   //Função pra imprimir traço
    int i;
        for (i = 0; i < 40; i++){
            printf("-");
        }
    printf("\n");
}

void cria_arquivos(FILE *arq_p, FILE *arq_g, FILE *arq_c){        // caso não haja arquivo, ele abrirá
            if (arq_p == NULL){
                arq_p = fopen("Pessoas.bin", "wb");
                fclose(arq_p);
            }
            if (arq_g == NULL){
                arq_g = fopen("Grupos.bin", "wb");
                fclose(arq_g);
            }
            if (arq_c == NULL){
                arq_c = fopen("Conversas.bin", "wb");
                fclose(arq_c);
            }
}

void remove_enter(char s[]){    //só tira o\n não sei se usarei, deixa ai por enquanto
    int tamanho = strlen(s);
    s[tamanho-1] = '\0';
}