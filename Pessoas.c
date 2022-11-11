#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pessoas.h"
#include "funcs_uteis.h"

void Salva_pessoas(Pessoa *p, int num_p){   //salva as pessoas no arquivo
    int i;
    FILE *arq_pessoas = fopen("Pessoas.bin", "wb");
        if(arq_pessoas != NULL){
        fwrite(&num_p, sizeof(int), 1, arq_pessoas);
        for (i = 0; i < num_p; i++){
            fwrite(&(p[i].id_p), sizeof(int), 1, arq_pessoas);
            fwrite(&(p[i].nome_p), LIM_MAX_NOME * sizeof(char), 1, arq_pessoas);
            fwrite(&(p[i].valido_p), sizeof(int), 1, arq_pessoas);
        }
    fclose(arq_pessoas);
        } else{
            perror("Escrita em arq_pessoas ");
        }
}

Pessoa * Traz_pessoas(int *num_p){
    int i;
    Pessoa *p;
    FILE *arq_pessoas = fopen("Pessoas.bin", "rb");
        if(arq_pessoas != NULL){
        fread(num_p, sizeof(int), 1, arq_pessoas);
        p = (Pessoa *) malloc((*num_p) * sizeof(Pessoa));
        for (i = 0; i < (*num_p); i++){
            fread(&(p[i].id_p), sizeof(int), 1, arq_pessoas);
            fread(&(p[i].nome_p), LIM_MAX_NOME * sizeof(char), 1, arq_pessoas);
            fread(&(p[i].valido_p), sizeof(int), 1, arq_pessoas);
        }
    fclose(arq_pessoas);
        } else{
            perror("Leitura de arq_pessoas ");
        }
    return p;
}

void cadastro_pessoa(Pessoa *p, int id){    //Função para cadastrar pessoas
    p->id_p = id;
    printf("Digite o nome da pessoa: ");
    fgets(p->nome_p, LIM_MAX_NOME, stdin);
    remove_enter(p->nome_p);
    p->valido_p = TRUE;
}

void troca_id(Pessoa *x, Pessoa *y){
    Pessoa tmp;
        tmp = *x;
        *x = *y;
        *y = tmp;
}

int partititon_id(Pessoa *v, int l, int r){
    int i, pivo = v[l].id_p, pos = r + 1;
        for (i = r; i >=l; i--){
            if(v[i].id_p >= pivo){
                pos--;
                troca_id(&(v[i]), &(v[pos]));
            }
        }
    return pos;
}

void quicksort_id(Pessoa *v, int l, int r){ //ordena id de pessoas
    int i;
        if(r<=l){
            return;
        }
        i = partititon_id(v,l,r);
        quicksort_id(v, l, i-1);
        quicksort_id(v, i+1, r);
}

void Bubblesort(Pessoa *p, int t){  //ordena nome de pessoas
    int i, j, valor;
    Pessoa aux;
        for (i = 0; i < t; i++){
           for (j = t-1; j > i; j--){
            valor = strcmp((p[j-1].nome_p), (p[j].nome_p));   //se valor>0, str2 é maior que a str1
                if(valor>0){
                    aux = p[j];
                    p[j] = p[j-1];
                    p[j-1] = aux;
                }
           } 
        }  
}

void lista_pessoas(Pessoa p[], int num){    //lista as pessoas
    int i;
        printf("\n\t    Oquerrr\n");
        imprime_traco();
        printf("|ID\t|Nome\n");
        imprime_traco();
        for(i = 0; i < num; i++){
            if(p[i].valido_p != TRUE){
                continue;
            }
            printf("|%d\t|%s\n", p[i].id_p, p[i].nome_p);
        }
        imprime_traco();
}

void ordem_pessoas(Pessoa *p, int num_p){   // junta as funções de ordenação
    int choice;
        do{
                printf("\n\t    Oquerrr\n");
                imprime_traco();
                printf("\t1 - Lista por ID\n\n\t2 - Lista A-Z\n\n\t3 - Voltar a sessao anterior\n> ");
                scanf("%d%*c", &choice);
                switch (choice)
                {
                case 3:                     //Voltar
                    break;
                case 1:                     
                    quicksort_id(p,0,num_p-1);   // Ordena por ID
                    lista_pessoas(p, num_p);
                    break;
                case 2:                     //Ordena Alfabeticamente
                    Bubblesort(p,num_p);
                    lista_pessoas(p,num_p);
                    quicksort_id(p,0,num_p-1);
                    break;  
                default:                    //Opção inválida
                    break;
                } 
            }while(choice != 3);
}

void remover(Pessoa *p){   //remove pessoas
    int id;
        printf("Qual pessoa deseja remover: ");
        scanf("%d", &id);
        p[id-1].valido_p = FALSE;
}

Pessoa *mod_pessoa(Pessoa *p, int *num_p){  // funções do módulo de pessoas
    int choice; 
        do{
            printf("\n\t\tOquerrr\n\t\t(Pessoas)\n");
            imprime_traco();
            printf("\t1 - Salvar Contato\n\n\t2 - Lista de Contatos\n\n\t3 - Remover Contato\n\n\t4 - Voltar sessao anterior\n> ");
            scanf("%d%*c", &choice);
            switch (choice)
            {
            case 4:                     //Voltar
                break;
            case 1:                     //Cadastrar
                (*num_p)++; //adiciona sempre o num de pessoas que tiver
                p = (Pessoa *) realloc(p, *num_p * sizeof(Pessoa));
                cadastro_pessoa(&(p[(*num_p) - 1]), *num_p);
                break;
            case 2:                     //Listar
                ordem_pessoas(p,(*num_p)); //Listagem
                break;  
            case 3:                     //Remover
                remover(p);
                break; 
            default:                    //Opção inválida
                break;
            } 
        }while(choice != 4);
        return p;
}