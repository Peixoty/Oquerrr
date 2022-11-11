#pragma once
#ifndef __grupos_h__
#define __grupos_h__

#include "Tipos.h"
#include "Pessoas.h"

typedef struct GRUPO{
   int id_g;
   char nome_g[LIM_MAX_NOME];
   int *npessoa;
   int num_pessoas;
   bool_t valido_g;
}Grupo;

Grupo *mod_grupo(Grupo *g, Pessoa *p, int *num_g, int num_p);

void Salva_grupos(Grupo *g, int num_g);

Grupo * Traz_grupos(int *num_g);

#endif //__grupos_h__