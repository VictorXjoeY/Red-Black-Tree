/* Victor Forbes - 9293394 */

#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "item.h"

typedef struct arvore_binaria ARVORE_BINARIA;

ARVORE_BINARIA *criar_arvore();
void apagar_arvore(ARVORE_BINARIA **);

int inserir_arvore(ARVORE_BINARIA *, ITEM *);
int remover_arvore(ARVORE_BINARIA *, int);

void print_arvore(ARVORE_BINARIA *);

#endif