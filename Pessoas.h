#pragma once
#ifndef __pessoas_h__
#define __pessoas_h__

#include "Tipos.h"

typedef struct PESSOAS{
   int id_p;
   char nome_p[LIM_MAX_NOME];
   bool_t valido_p;
}Pessoa;

Pessoa *mod_pessoa(Pessoa *p, int *num_p);

void Salva_pessoas(Pessoa *p, int num_p);

Pessoa * Traz_pessoas(int *num_p);

void lista_pessoas(Pessoa p[], int num);

#endif //__pessoas_h__