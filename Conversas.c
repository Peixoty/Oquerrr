#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Conversas.h"
#include "Pessoas.h"
#include "Grupos.h"
#include "Tipos.h"
#include "funcs_uteis.h"

//As mensagens serão guardadas em um pointer pointer, em que cada posição do primeiro nivel representa uma mensagem
//e cada posição do segundo nível representa a conversa à qual pertencem as mensagens.
//Pra isso serve o ID das conversas: sincronizar cada conversa com seu respectivo pointer de mensagens.

void Salva_conversas(Conversa *c, int num_conversas){
    int i, j;
    FILE *arq_conversas = fopen("Conversas.bin", "wb");
        if(arq_conversas != NULL){
        fwrite(&num_conversas, sizeof(int), 1, arq_conversas);
        for (i = 0; i < num_conversas; i++){
            fwrite(&(c[i].ID_conversa), sizeof(int), 1, arq_conversas);
            fwrite(&(c[i].ID_pessoa1), sizeof(int), 1, arq_conversas);
            fwrite(&(c[i].ID_pessoa2), sizeof(int), 1, arq_conversas);
            fwrite(&(c[i].numMensagens), sizeof(int), 1, arq_conversas);
                for (j = 0; j < c[i].numMensagens; j++){
                    fwrite(&(c[i].texto[j].id_c), sizeof(int), 1, arq_conversas);
                    fwrite(&(c[i].texto[j].ID_remetente), sizeof(int), 1, arq_conversas);
                    fwrite(&(c[i].texto[j].mensagem), LIM_MAX_MSG * sizeof(char), 1, arq_conversas);
                    fwrite(&(c[i].texto[j].valido_c), sizeof(int), 1, arq_conversas);
                    fwrite(&(c[i].texto[j].apagavel), sizeof(int), 1, arq_conversas);
                }
        }
    fclose(arq_conversas);
        } else{
            perror("Escrita em arq_conversas ");
        }
}

Conversa * Traz_conversas(int *num_conversas){
    int i, j;
    FILE *arq_conversas = fopen("Conversas.bin", "rb");
    Conversa *c;
        if(arq_conversas != NULL){
        fread(num_conversas, sizeof(int), 1, arq_conversas);
        c = (Conversa *) malloc((*num_conversas) * sizeof(Conversa));
        for (i = 0; i < *num_conversas; i++){
            fread(&(c[i].ID_conversa), sizeof(int), 1, arq_conversas);
            fread(&(c[i].ID_pessoa1), sizeof(int), 1, arq_conversas);
            fread(&(c[i].ID_pessoa2), sizeof(int), 1, arq_conversas);
            fread(&(c[i].numMensagens), sizeof(int), 1, arq_conversas);
            c[i].texto = (Mensagens *) malloc(c[i].numMensagens * sizeof(Mensagens));
                for (j = 0; j < c[i].numMensagens; j++){
                    fread(&(c[i].texto[j].id_c), sizeof(int), 1, arq_conversas);
                    fread(&(c[i].texto[j].ID_remetente), sizeof(int), 1, arq_conversas);
                    fread(&(c[i].texto[j].mensagem), LIM_MAX_MSG * sizeof(char), 1, arq_conversas);
                    fread(&(c[i].texto[j].valido_c), sizeof(int), 1, arq_conversas);
                    fread(&(c[i].texto[j].apagavel), sizeof(int), 1, arq_conversas);
                }
        }
    fclose(arq_conversas);
        } else{
            perror("Escrita em arq_conversas ");
        }
    return c;
}

Conversa * cria_conversa_privada(Conversa* conversa_pntr, Pessoa * pessoa, int *num_conversaAtual, int num_p) {
    int i, id_1, id_2;
        (*num_conversaAtual)++;
        printf("\n----Insira os IDs das duas pessoas que farao parte da conversa----\n");
            printf("Id 1\n> ");
            scanf("%d", &id_1);
            printf("Id 2\n> ");
            scanf("%d", &id_2);

        //Caso seja um id Falso ou um além do numero de pessoas ele não deixa abrir a mensagem
        if(id_1 > num_p || pessoa[id_1 - 1].valido_p == FALSE){
            printf("O primeiro ID eh desconhecido\n\n");
            (*num_conversaAtual)--;
            return conversa_pntr;
        } else if(id_2 > num_p || pessoa[id_2 - 1].valido_p == FALSE){
            printf("O segundo ID eh desconhecido\n\n");
            (*num_conversaAtual)--;
            return conversa_pntr;
        }
        else{   // se nenhum dos IDs extrapola a qntd de pessoa ou ja tenha sido removido, ele confere se a conversa ja existe. Conversa 1 e 2/ 2 e 1 sao iguais
            for(i=0; i < *num_conversaAtual - 1; i++){
                if(conversa_pntr[i].ID_pessoa1 == id_1 || conversa_pntr[i].ID_pessoa1 == id_2 ){
                    if(conversa_pntr[i].ID_pessoa2 == id_2 || conversa_pntr[i].ID_pessoa2 == id_1){
                        printf("Conversa ja existente\n\n");
                        (*num_conversaAtual)--;
                        return conversa_pntr;
                    }
                }
            }
        }
        conversa_pntr = (Conversa *) realloc(conversa_pntr, (*num_conversaAtual) * sizeof(Conversa));
        conversa_pntr[(*num_conversaAtual)-1].ID_conversa = *num_conversaAtual; //identifica o ID da conversa, ou seja, a posição
                                                                        //da conversa no vetor de conversas
        conversa_pntr[(*num_conversaAtual - 1)].ID_pessoa1 = id_1;
        conversa_pntr[(*num_conversaAtual - 1)].ID_pessoa2 = id_2; 
        conversa_pntr[(*num_conversaAtual)-1].numMensagens = 0;
        conversa_pntr[(*num_conversaAtual)-1].texto = NULL; 
    return conversa_pntr;
}

Conversa * cria_conversa_grupo(Conversa* conversa_pntr, Grupo * grupo, int *num_conversaAtual, int num_g) {
    int i, id_1, id_2;
        (*num_conversaAtual)++;
        printf("\n----Insira o ID do grupo----\n> ");
        scanf("%d", &id_1);
        id_2 = -1;
        //Caso seja um id Falso ou um além do numero de pessoas ele não deixa abrir a mensagem
        if(id_1 > num_g || grupo[id_1 - 1].valido_g == FALSE){
            printf("Grupo nao encontrado\n\n");
            (*num_conversaAtual)--;
            return conversa_pntr;
        } else{     //se nenhum dos IDs extrapola a qntd de pessoa ou ja tenha sido removido, ele confere se a conversa ja existe.
            for(i=0; i < *num_conversaAtual - 1; i++){
                if(conversa_pntr[i].ID_pessoa1 == id_1){
                    if(conversa_pntr[i].ID_pessoa2 == -1){
                        printf("Essa conversa de grupo ja existe\n\n");
                        conversa_pntr = (Conversa *) realloc(conversa_pntr, ((*num_conversaAtual)-1) * sizeof(Conversa));
                        (*num_conversaAtual)--;
                        return conversa_pntr;
                    }
                }
            }
        }
        conversa_pntr = (Conversa *) realloc(conversa_pntr, (*num_conversaAtual) * sizeof(Conversa));
        conversa_pntr[(*num_conversaAtual)-1].ID_conversa = *num_conversaAtual; //identifica o ID da conversa, ou seja, a posição
                                                                        //da conversa no vetor de conversas
        conversa_pntr[(*num_conversaAtual)-1].ID_pessoa1 = id_1;                                                                
        conversa_pntr[(*num_conversaAtual)-1].ID_pessoa2 = id_2;  // identifica q se trata de um grupo. OBS: não há ID -1
        conversa_pntr[(*num_conversaAtual)-1].numMensagens = 0; 
        conversa_pntr[(*num_conversaAtual)-1].texto = NULL; 
    return conversa_pntr;
}

void lista_conversas(Pessoa *pessoa, Grupo * grupo, Conversa *conversa, int num_conversas, int var_controle){
    int i;
        printf("\n\t    Oquerrr\n");
        imprime_traco();
        printf("|ID\t|Conversa\n");
        imprime_traco();
        if(var_controle < 0){   //Conversas de grupos
            for (i = 0; i < num_conversas; i++){
                if(conversa[i].ID_pessoa2 > 0 || grupo[i].valido_g == FALSE){
                    continue;
                } else{
                    printf("|%d\t|%s\n", conversa[i].ID_conversa, grupo[conversa[i].ID_pessoa1 - 1].nome_g);
                }
            }
            imprime_traco();
        } else{ //Conversa de pessoas
            for (i = 0; i < num_conversas; i++){
                if(conversa[i].ID_pessoa2 < 0){
                    continue;
                } else{
                    printf("|%d\t|%s - %s\n", conversa[i].ID_conversa, pessoa[conversa[i].ID_pessoa1 - 1].nome_p, pessoa[conversa[i].ID_pessoa2 - 1].nome_p);
                }
            }
            imprime_traco();
        }
        
}   

void imprime_mensagem(Pessoa *p, Grupo * g, Conversa *c){
    int i;
        if(c->ID_pessoa2>0){
            printf("\n\t\tOquerrr\n\t%7s|%s - %s|\n", "", p[c->ID_pessoa1-1].nome_p, p[c->ID_pessoa2-1].nome_p);
            imprime_traco();
            for(i=0; i < c->numMensagens; i++){
                if(c->texto[i].valido_c == FALSE){
                    continue;
                }
                printf("[%d] - [%s]: %s\n", c->texto[i].id_c, p[c->texto[i].ID_remetente -1].nome_p, c->texto[i].mensagem);
            }
        } else{
            printf("\n\t\tOquerrr\n\t%7s|%s|\n", "", g[c->ID_pessoa1-1].nome_g);
            imprime_traco();
            for(i=0; i < c->numMensagens; i++){
                if(c->texto[i].valido_c == FALSE){
                    continue;
                }
                printf("[%d] - [%s]: %s\n", c->texto[i].id_c, p[c->texto[i].ID_remetente -1].nome_p, c->texto[i].mensagem);
            }
        }
}

void invalida_mensagem(Conversa *conversa, int id_mensagem){

        if(conversa->texto[id_mensagem-1].apagavel == TRUE){
            conversa->texto[id_mensagem-1].valido_c = FALSE;
        } else{
            printf("Essa mensagem nao pode ser mais apagada");
        }
}

Conversa * manda_mensagem_pv(Conversa * conversa, Pessoa * pessoa, Grupo * grupo, int id){
    int id_r, i;
        imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
        printf("\n\n<ID remetente> <Mensagem> - Envia Mensagem de ate 100 Caracteres\n\n"
        "-<ID mensagem> Apaga Mensagem - Apaga mensagem\n\n"
        "<O> Volta para a pagina anterior\n\n");
        do{
        printf("> ");
        scanf("%d%*c", &id_r);
        if((id_r>0) && ((id_r != conversa[id-1].ID_pessoa1) && (id_r != conversa[id-1].ID_pessoa2))){
            printf("Esse id eh invalido ou inexistente nessa conversa\n--Tente Novamente--\n");
            fflush(stdin);  //Libera o stdin
            continue;
        } else if((id_r>0) && (pessoa[id_r-1].valido_p == FALSE)){
            printf("Esse id eh invalido ou inexistente nessa conversa\n--Tente Novamente--\n");
            fflush(stdin);  //Libera o stdin
            continue;
        }
        if(id_r > 0){
            conversa[id-1].numMensagens++;
            conversa[id-1].texto = (Mensagens *) realloc(conversa[id-1].texto, conversa[id-1].numMensagens * sizeof(Mensagens));
            conversa[id-1].texto[conversa[id-1].numMensagens-1].id_c = conversa[id-1].numMensagens;
            conversa[id-1].texto[conversa[id-1].numMensagens-1].apagavel = TRUE;
            conversa[id-1].texto[conversa[id-1].numMensagens-1].ID_remetente = id_r;
            fgets(conversa[id-1].texto[conversa[id-1].numMensagens-1].mensagem, LIM_MAX_MSG, stdin);
            remove_enter(conversa[id-1].texto[conversa[id-1].numMensagens-1].mensagem);
            conversa[id-1].texto[conversa[id-1].numMensagens-1].valido_c = TRUE;
            imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
        } else if(id_r < 0){
            invalida_mensagem(&(conversa[id-1]), id_r * -1);
            imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
        } else{
            for(i=0; i<conversa[id-1].numMensagens; i++){
                conversa[id-1].texto[i].apagavel = FALSE;
            }
            return conversa;
        }
        }while(TRUE == TRUE);
    return conversa;
}

Conversa * manda_mensagem_gp(Conversa * conversa, Pessoa* pessoa, Grupo * grupo, int id, int num_p){
int id_r, i, num;
        imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
        printf("\n\n<ID remetente> <Mensagem> - Envia Mensagem de ate 100 Caracteres\n\n"
        "-<ID mensagem> Apaga Mensagem - Apaga mensagem\n\n"
        "<O> Volta para a pagina anterior\n\n");
        do{
        num = 0;
        printf("> ");
        scanf("%d%*c", &id_r);
        if(id_r>num_p){
            printf("Esse id eh invalido ou inexistente nessa conversa\n--Tente Novamente--\n");
            fflush(stdin);  //Libera o stdin
            continue;
        }
        if(id_r>0){
            for (i = 0; i < grupo[conversa[id-1].ID_pessoa1-1].num_pessoas; i++){
                if((id_r>0) && (id_r == grupo[conversa[id-1].ID_pessoa1-1].npessoa[i])){
                    if (pessoa[id_r-1].valido_p == TRUE){
                        num = 0;
                        break;
                    } else{
                        num++;  //Pessoa é invalida
                    }
                } else{
                    num++;  // Pessoa nao esta no gp
                }
            }
            if(num>0){  //caso o id seja falso
                printf("Esse id eh invalido ou inexistente nessa conversa\n--Tente Novamente--\n");
                fflush(stdin);  //Libera o stdin
                continue;
            }
        }
            if(id_r > 0){
                conversa[id-1].numMensagens++;
                conversa[id-1].texto = (Mensagens *) realloc(conversa[id-1].texto, conversa[id-1].numMensagens * sizeof(Mensagens));
                conversa[id-1].texto[conversa[id-1].numMensagens-1].id_c = conversa[id-1].numMensagens;
                conversa[id-1].texto[conversa[id-1].numMensagens-1].apagavel = TRUE;
                conversa[id-1].texto[conversa[id-1].numMensagens-1].ID_remetente = id_r;
                fgets(conversa[id-1].texto[conversa[id-1].numMensagens-1].mensagem, LIM_MAX_MSG, stdin);
                remove_enter(conversa[id-1].texto[conversa[id-1].numMensagens-1].mensagem);
                conversa[id-1].texto[conversa[id-1].numMensagens-1].valido_c = TRUE;
                imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
            } else if(id_r < 0){
                invalida_mensagem(&(conversa[id-1]), id_r * -1);
                imprime_mensagem(pessoa, grupo, &(conversa[id-1]));
            } else{
                for(i=0; i<conversa[id-1].numMensagens; i++){
                    conversa[id-1].texto[i].apagavel = FALSE;
                }
                return conversa;
            }
        }while(TRUE == TRUE);
    return conversa;
}

Conversa * abre_conversa(Conversa* conversa_pntr, Pessoa * pessoa, Grupo *grupo, int num_conversas) {
    int id;
        printf("Digite o id da conversa\n> ");
        scanf("%d", &id);
        if(id <= 0){
            printf("Essa conversa nao existe");
            return conversa_pntr;
        } else if(id > num_conversas || conversa_pntr[id-1].ID_pessoa2 == -1){
            printf("Essa conversa nao existe");
            return conversa_pntr;
        }
        conversa_pntr = manda_mensagem_pv(conversa_pntr, pessoa, grupo, id);
    return conversa_pntr;
}

Conversa * abre_grupo(Conversa* conversa_pntr, Pessoa *pessoa, Grupo *grupo, int num_conversas, int num_p) {
    int id;
        printf("Digite o id da conversa do grupo\n> ");
        scanf("%d", &id);
        if(id <= 0 || id > num_conversas){
            printf("Esse grupo nao existe");
            return conversa_pntr;
        } else if(conversa_pntr[id-1].ID_pessoa2 != -1 || grupo[id-1].valido_g == FALSE){
            printf("Esse grupo nao existe");
            return conversa_pntr;
        }
        conversa_pntr = manda_mensagem_gp(conversa_pntr, pessoa, grupo, id, num_p);

    return conversa_pntr;
}

Conversa * qual_conversa(Pessoa * pessoas, Grupo *grupo, Conversa * conversa, int *num_conversas, int num_p, int num_g){
    int opcao;
        printf("\n\t\tOquerrr\n\t\t(Conversas)\n");
        imprime_traco();
        printf("\t1 - Conversa Privada\n\n\t2 - Conversa de Grupo\n\n\t3 - Voltar sessao anterior\n> ");
        scanf("%d%*c", &opcao);
            switch (opcao){
                case 3:
                    break;
                case 1:
                    conversa = cria_conversa_privada(conversa, pessoas, num_conversas, num_p);
                    break;
                case 2:
                    conversa = cria_conversa_grupo(conversa, grupo, num_conversas, num_g);
                break;
                default:
                    break;
            }
    return conversa;
}

Conversa * mod_conversas(Pessoa * pessoas, Grupo *grupo, Conversa * conversa, int *num_conversas, int num_p, int num_g) {
    int opcao = 0;
        do {
            printf("\n\t\tOquerrr\n\t\t(Conversas)\n");
            imprime_traco();
            printf("\t1 - Criar Conversa\n\n\t2 - Abrir Conversa Privada\n\n\t3 - Abrir Grupo\n\n\t4 - Voltar sessao anterior\n> ");
            scanf("%d%*c", &opcao);
            switch(opcao) {
            case 4:
                break;
            case 1: 
                conversa = qual_conversa(pessoas,grupo,conversa,num_conversas, num_p, num_g);
                break;
            case 2:
                lista_conversas(pessoas,grupo,conversa,*num_conversas,1);
                conversa = abre_conversa(conversa, pessoas, grupo, *num_conversas);
                break;
            case 3:
                lista_conversas(pessoas,grupo,conversa,*num_conversas,-1);
                conversa = abre_grupo(conversa,pessoas, grupo, *num_conversas, num_p);
                break;
            }
        } while (opcao != 4);
    return conversa;
}