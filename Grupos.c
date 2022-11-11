#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Grupos.h"
#include "funcs_uteis.h"

void Salva_grupos(Grupo *g, int num_g){   //salva as pessoas no arquivo
    int i, j;
    FILE *arq_grupos = fopen("Grupos.bin", "wb");
        if(arq_grupos != NULL){
        fwrite(&num_g, sizeof(int), 1, arq_grupos);
        for (i = 0; i < num_g; i++){
            fwrite(&(g[i].id_g), sizeof(int), 1, arq_grupos);
            fwrite(&(g[i].nome_g), LIM_MAX_NOME * sizeof(char), 1, arq_grupos);
            fwrite(&(g[i].num_pessoas), sizeof(int), 1, arq_grupos);
                for (j = 0; j < g[i].num_pessoas; j++){
                    fwrite(&(g[i].npessoa[j]), sizeof(int), 1, arq_grupos);
                }
                
            fwrite(&(g[i].valido_g), sizeof(int), 1, arq_grupos);
        }
    fclose(arq_grupos);
        } else{
            perror("Escrita em arq_grupos ");
        }
}

Grupo * Traz_grupos(int *num_g){
    int i, j;
    Grupo *g;
    FILE *arq_grupos = fopen("Grupos.bin", "rb");
        if(arq_grupos != NULL){
        fread(num_g, sizeof(int), 1, arq_grupos);
        g = (Grupo *) malloc((*num_g) * sizeof(Grupo));
        for (i = 0; i < (*num_g); i++){
            fread(&(g[i].id_g), sizeof(int), 1, arq_grupos);
            fread(&(g[i].nome_g), LIM_MAX_NOME * sizeof(char), 1, arq_grupos);
            fread(&(g[i].num_pessoas), sizeof(int), 1, arq_grupos);
            g[i].npessoa = (int *) malloc(g[i].num_pessoas * sizeof(int));
                for (j = 0; j < g[i].num_pessoas; j++){
                    fread(&(g[i].npessoa[j]), sizeof(int), 1, arq_grupos);
                }
                
            fread(&(g[i].valido_g), sizeof(int), 1, arq_grupos);
        }
    fclose(arq_grupos);
        } else{
            perror("Leitura de arq_grupos ");
        }
    return g;
}

void Insere_pessoas(Grupo *g, Pessoa *p, int num_p, int i){
    int j;
        do{
            (g->num_pessoas)++;
            g->npessoa = (int *) realloc(g->npessoa, g->num_pessoas * sizeof(int));
            lista_pessoas(p,num_p);
            printf("ID's que quer adicionar(0 para parar)\n> ");
            scanf("%d", &(g->npessoa[i]));
            for(j=0;j<g->num_pessoas-1;j++){
                if(g->npessoa[j] == g->npessoa[i]){
                    printf("<Pessoa %d ja adicionada>\n", g->npessoa[i]);
                    (g->num_pessoas)--;
                    g->npessoa = (int *) realloc(g->npessoa, g->num_pessoas * sizeof(int));
                i--;
                }
            }
            if(g->npessoa[i] == 0){
                break;
            }
            if (g->npessoa[i]>num_p || p[g->npessoa[i] - 1].valido_p == FALSE){
                if(i==0){
                    printf("<Pessoa %d nao existe>\n",g->npessoa[i]);
                    g->npessoa = (int *) realloc(g->npessoa, i * sizeof(int));
                    g->npessoa = (int *) malloc(1*sizeof(int));
                    (g->num_pessoas)--;
                    i--;
                }else{
                    (g->num_pessoas)--;
                    printf("<Pessoa %d nao existe>\n",g->npessoa[i]);
                    g->npessoa = (int *) realloc(g->npessoa, g->num_pessoas * sizeof(int));
                i--;
                }
            } 
            i++;
        }while(g->npessoa[i-1] != 0);
        if(g->npessoa[i] == 0){
                (g->num_pessoas)--;
                g->npessoa = (int *) realloc(g->npessoa, g->num_pessoas * sizeof(int));
        }
}

void cadastro_grupo(Grupo *g, Pessoa *p, int id, int num_p){    //Função para cadastrar Grupos
        g->npessoa = (int *) malloc(1*sizeof(int));
        g->id_g = id;
        printf("Digite o nome do grupo: ");
        fgets(g->nome_g, LIM_MAX_NOME, stdin);
        remove_enter(g->nome_g);
        g->valido_g = TRUE;
        g->num_pessoas = 0;
        Insere_pessoas(g,p,num_p, 0);
}

void troca_id_g(Grupo *x, Grupo *y){
    Grupo tmp;
        tmp = *x;
        *x = *y;
        *y = tmp;
}

int partititon_id_g(Grupo *v, int l, int r){
    int i, pivo = v[l].id_g, pos = r + 1;
        for (i = r; i >=l; i--){
            if(v[i].id_g >= pivo){
                pos--;
                troca_id_g(&(v[i]), &(v[pos]));
            }
        }
    return pos;
}

void quicksort_id_g(Grupo *v, int l, int r){    //ordena ID dos grupos
    int i;
        if(r<=l){
            return;
        }
        i = partititon_id_g(v,l,r);
        quicksort_id_g(v, l, i-1);
        quicksort_id_g(v, i+1, r);
}

void Bubblesort_grupo(Grupo *g, int t){ //ordena nome dos grupos
    int i, j, valor;
    Grupo aux;
        for (i = 0; i < t; i++){
            for (j = t-1; j > i; j--){
                valor = strcmp((g[j-1].nome_g), (g[j].nome_g));   //se valor>0, str2 é maior que a str1
                if(valor>0){
                    aux = g[j];
                    g[j] = g[j-1];
                    g[j-1] = aux;
                }
           } 
        }  
}

void Bubblesort_pessoas_grupo(Pessoa *p, int *id_pessoas, int t){   //ordena o vetor npessoas alfabeticamente para listagem alfabetica dentro do grupo
    int i, j, valor;
    int aux;
        for (i = 0; i < t; i++){
            for (j = t-1; j > i; j--){
                valor = strcmp((p[id_pessoas[j-1]-1].nome_p), (p[id_pessoas[j]-1].nome_p));   //se valor>0, str2 é maior que a str1
                if(valor>0){
                    aux = id_pessoas[j];
                    id_pessoas[j] = id_pessoas[j-1];
                    id_pessoas[j-1] = aux;
                }
            } 
        }  
}

void lista_grupo(Grupo *g, int num){    //lista os grupos
    int i;
        printf("\n\t    Oquerrr\n");
        imprime_traco();
        printf("|ID\t|Nome\n");
        imprime_traco();
        for(i = 0; i < num; i++){
            if(g[i].valido_g != TRUE){
                continue;
            }
            printf("|%d\t|%s\n", g[i].id_g, g[i].nome_g);
        }
        imprime_traco();
}

void lista_pessoas_grupo(Grupo *g, Pessoa *p, int num_g, int num_p){   //lista as pessoas no grupo
    int i, j, id;
        printf("Id do grupo desejado: ");
        scanf("%d", &id);
            if(id > num_g || g[id-1].valido_g == FALSE){
                printf("Esse grupo nao existe");
                    return;
                }
            Bubblesort_pessoas_grupo(p, g[id-1].npessoa, g[id-1].num_pessoas);
            printf("\t%s\n", g[id-1].nome_g);
            imprime_traco();
            for (i = 0; i < g[id-1].num_pessoas; i++){
                for (j = 0; j < num_p; j++){
                    if(p[j].id_p == g[id-1].npessoa[i]){
                        if(p[j].valido_p != TRUE){
                            printf("|%d\t|%s\n", p[j].id_p, "Desconhecido");
                            imprime_traco();
                            continue;
                        }
                        printf("|%d\t|%s\n", p[j].id_p, p[j].nome_p);
                        imprime_traco();
                    }
                }
            }
}

void remover_grupo(Grupo *g){  //remove p grupo
    int id;
        printf("ID do grupo deseja remover: ");
        scanf("%d", &id);
        g[id-1].valido_g = FALSE;
}

void ordem_grupo(Grupo *g, Pessoa *p, int num_g, int num_p){    //junta as funções de ordenação do grupo
    int choice; 
        do{
                printf("\n\t    Oquerrr\n");
                imprime_traco();
                printf("\t1 - Lista por ID\n\n\t2 - Lista A-Z\n\n\t3 - Pessoas do Grupo\n\n\t4 - Voltar a sessao anterior\n> ");
                scanf("%d%*c", &choice);
                switch (choice)
                {
                case 4:                     //Voltar
                    break;
                case 1:                     
                    quicksort_id_g(g,0,num_g-1);   // Ordena por ID
                    lista_grupo(g, num_g);
                    break;
                case 2:                     //Ordena Alfabeticamente
                    Bubblesort_grupo(g, num_g);
                    lista_grupo(g, num_g);
                    quicksort_id_g(g,0,num_g-1);    //para não atrapalhar o detalhamento
                    break; 
                case 3:
                    lista_pessoas_grupo(g,p,num_g,num_p);
                default:                    //Opção inválida
                    break;
                } 
            }while(choice != 4);
}

Grupo *mod_grupo(Grupo *g, Pessoa *p, int *num_g, int num_p){   //funções de módulo de grupo
    int choice, id_grupo; 
        do{
            printf("\n\t\tOquerrr\n\t\t(Grupos)\n");
            imprime_traco();
            printf("\t1 - Criar Grupo\n\n\t2 - Lista de Grupos\n\n\t3 - Remove Grupos\n\n\t4 - Adiciona Pessoas\n\n\t5 - Voltar sessao anterior\n> ");
            scanf("%d%*c", &choice);
            switch (choice)
            {
            case 5:                     //Voltar
                break;
            case 1:                     //Cadastrar
                (*num_g)++; //adiciona sempre o num de pessoas que tiver
                g = (Grupo *) realloc(g, *num_g * sizeof(Grupo));
                cadastro_grupo(&(g[(*num_g) - 1]), p, *num_g, num_p);
                break;
            case 2:                      //Listar
                ordem_grupo(g, p,(*num_g), num_p); //Listagem
                break;  
            case 3:                     //Remover
                remover_grupo(g);
                break; 
            case 4:
                printf("Digite o id do grupo que deseja adicionar pessoas: ");
                scanf("%d", &id_grupo);
                    if(id_grupo > (*num_g)){
                    printf("Esse grupo nao existe");
                    break;
                    }
                Insere_pessoas(&(g[id_grupo-1]),p,num_p, g[id_grupo-1].num_pessoas);
                break;
            default:                    //Opção inválida
                break;
            }
        }while(choice != 5);
        return g;
}