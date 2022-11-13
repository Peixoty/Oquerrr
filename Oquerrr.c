#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pessoas.h"
#include "Grupos.h"
#include "Conversas.h"
#include "funcs_uteis.h"
#define SUCESSO 0

int main(int argc, char ** argv)
{
    FILE *pessoas = fopen("Pessoas.bin", "rb");
    FILE *grupos = fopen("Grupos.bin", "rb");
    FILE *conversas = fopen("Conversas.bin", "rb");
    int choice, n_pessoas = 0, n_grupos = 0, n_conversas = 0, i;
    Pessoa *p = NULL;
    Grupo *g = NULL;
    Conversa *c = NULL;
        if(pessoas == NULL || grupos == NULL || conversas == NULL){
            cria_arquivos(pessoas, grupos, conversas);
        }
        p = Traz_pessoas(&n_pessoas);
        g = Traz_grupos(&n_grupos);
        c = Traz_conversas(&n_conversas);
        fclose(pessoas);
        fclose(grupos);
        fclose(conversas);
        do{
            printf("\n\t\tOquerrr\n");
            imprime_traco();
            printf("\t1 - Pessoas\n\n\t2 - Grupos\n\n\t3 - Conversas\n\n\t4 - Sair do App\n> ");
            scanf("%d%*c", &choice);
            switch (choice)
            {
            case 4:                             //Voltar
                break;
            case 1:                             //Módulo de Pessoas
                p = mod_pessoa(p, &n_pessoas);  //passo como pointer para modificar em todo o programa
                break;
            case 2:                             //Módulo de Grupos
                g = mod_grupo(g, p, &n_grupos, n_pessoas);
                break;  
            case 3:
                c = mod_conversas(p, g, c, &n_conversas, n_pessoas, n_grupos);               //Módulo de Conversas
                break; 
            default:                    //Opção inválida
                break;
            }
        }while(choice != 4);
        Salva_pessoas(p, n_pessoas);
        Salva_grupos(g, n_grupos);
        Salva_conversas(c, n_conversas);
        free(p);
        
        for (i = 0; i < n_grupos; i++) {    //desaloca o npessoas
            free(g[i].npessoa);
        }
        free(g);

        for (i = 0; i < n_conversas; i++) { //desaloca os textos
            free(c[i].texto);
        }
        free(c);
        printf("Backup feito com sucesso\n");
        printf("Saindo...\n");
    return SUCESSO;
}