/* Victor Forbes - 9293394 

Material usado para desenvolver a função de remoção:
    - http://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
    - https://www.youtube.com/watch?v=7CesCbbVxqc 
*/

#include <stdlib.h>
#include <stdio.h>

#include "arvore_binaria.h"
#include "item.h"

#define true 1
#define false 0

#define FESQ 0
#define FDIR 1

#define VERMELHO 0
#define PRETO 1

#define FILHO 0
#define PAI 1
#define AVO 2

typedef unsigned char bool;

typedef struct NO{
    struct NO *fesq;
    struct NO *fdir;
    ITEM *item;
    bool cor;
}NO;

struct arvore_binaria{
    NO *raiz;
};

ARVORE_BINARIA *criar_arvore(){
    ARVORE_BINARIA *arv = (ARVORE_BINARIA *)malloc(sizeof(ARVORE_BINARIA));

    if (arv){
        arv->raiz = NULL;
    }

    return arv;
}

void apagar_arvore_aux(NO *raiz){
    if (raiz){
        apagar_arvore_aux(raiz->fesq);
        apagar_arvore_aux(raiz->fdir);
        apagar_item(&raiz->item);
        free(raiz);
    }
}

void apagar_arvore(ARVORE_BINARIA **arv){
    if (arv && *arv){
        apagar_arvore_aux((*arv)->raiz);
        free(*arv);
        *arv = NULL;
    }
}

/* Função que cria e inicializa um nó. */
NO *criar_no(ITEM *item){
    NO *no = NULL;

    if (item){
        no = (NO *)malloc(sizeof(NO));

        if (no){
            no->fesq = NULL;
            no->fdir = NULL;
            no->item = item;
            no->cor = VERMELHO;
        }
    }
    
    return no;
}

/* Função que libera a memória alocada para armazenar um nó. */
void apagar_no(NO *no){
    if (no){
        apagar_item(&(no->item));
        free(no);
    }
}

/* Função que troca o conteúdo de dois nós. */
void swap_content(NO *a, NO *b){
    ITEM *aux = a->item;
    a->item = b->item;
    b->item = aux;
}

/* Função que realiza a rotação simples-direita. */
NO *rot_direita(NO *a){
    NO *b = a->fesq;

    a->fesq = b->fdir;
    b->fdir = a;

    // Atualizando as cores.
    b->cor = PRETO;
    a->cor = VERMELHO;

    return b;
}

/* Função que realiza a rotação simples-esquerda. */
NO *rot_esquerda(NO *a){
    NO *b = a->fdir;

    a->fdir = b->fesq;
    b->fesq = a;

    // Atualizando as cores.
    b->cor = PRETO;
    a->cor = VERMELHO;

    return b;
}

/* Função que realiza a rotação dupla-direita (esquerda > direita). */
NO *rot_dupla_direita(NO *a){
    a->fesq = rot_esquerda(a->fesq);
    return rot_direita(a);
}

/* Função que realiza a rotação dupla-esquerda (direita > esquerda). */
NO *rot_dupla_esquerda(NO *a){
	a->fdir = rot_direita(a->fdir);
	return rot_esquerda(a);
}

NO *inserir_arvore_aux(NO *raiz, ITEM *item, int *aux){
    // Se a raiz for NULL, encontrei a posição de inserção.
    if (raiz == NULL){
        *aux = FILHO;
        return criar_no(item);
    }

    // Insiro a esquerda se for verdadeiro e a direita se for falso.
    if (item->chave < raiz->item->chave){
        // Chamando para o filho da esquerda.
        raiz->fesq = inserir_arvore_aux(raiz->fesq, item, aux);

        // Caso o filho do nó atual seja o pai do nó inserido e seja vermelho,
        // tenho que aplicar alguma operação (recoloração ou rotação).
        if (*aux == PAI && raiz->fesq->cor == VERMELHO){
            // Se o tio do nó inserido for vermelho, a operação é recoloração.
            // A operação é rotação caso contrário.
            if (raiz->fdir && raiz->fdir->cor == VERMELHO){
                raiz->fesq->cor = PRETO;
                raiz->fdir->cor = PRETO;
                raiz->cor = VERMELHO;

                // Atualizando aux para considerar o nó atual como "novo nó de inserção".
                *aux = FILHO;

                // Retornando para não entrar em conflito com o (*aux)++.
                return raiz;
            }
            else{
                // Checando se a rotação é simples ou dupla.
                if (item->chave < raiz->fesq->item->chave){
                    // Rotação simples para a direita.
                    raiz = rot_direita(raiz);
                }
                else{
                    // Rotação dupla: primeiro para a esquerda e depois para a direita.
                    raiz = rot_dupla_direita(raiz);
                }
            }
        }
    }
    else if (item->chave > raiz->item->chave){ // Operações análogas para o lado direito.
        raiz->fdir = inserir_arvore_aux(raiz->fdir, item, aux);

        if (*aux == PAI && raiz->fdir->cor == VERMELHO){
            if (raiz->fesq && raiz->fesq->cor == VERMELHO){
                raiz->fesq->cor = PRETO;
                raiz->fdir->cor = PRETO;
                raiz->cor = VERMELHO;

                *aux = FILHO;

                return raiz;
            }
            else{
                if (item->chave < raiz->fdir->item->chave){
                    raiz = rot_dupla_esquerda(raiz);
                }
                else{
                    raiz = rot_esquerda(raiz);
                }
            }
        }
    }
    else{
        // Se já houver um item com a mesma chave na árvore, apenas substitua o item.
        apagar_item(&(raiz->item));
        raiz->item = item;
    }

    // Incrementando o conteúdo apontado por aux para que a chamada anterior
    // tenha a relação de parentesco dos nós abaixo.
    (*aux)++;

    return raiz;
}

int inserir_arvore(ARVORE_BINARIA *arv, ITEM *item){
    int aux = AVO;

    if (arv){
        arv->raiz = inserir_arvore_aux(arv->raiz, item, &aux);

        if (arv->raiz){
            arv->raiz->cor = PRETO;
        }
    }

    return arv && arv->raiz != NULL;
}

NO *remover_arvore_aux(NO *raiz, int chave, char *aux, NO *z){
    NO *p, *v, *u, *w; // p = pai | v = nó a ser removido | u = nó que substitui o nó removido | w = irmão de u
    bool dir, vcolor, flag; // dir = direção seguida | vcolor = auxiliar para salvar cores | flag = auxiliar

    if (raiz == NULL){
        // Não achei o nó de remoção.
        return NULL;
    }

    // Se o conteúdo apontado por aux for 0, estou no modo de busca pelo nó de chave "chave".
    if (*aux == 0){
        if (chave < raiz->item->chave){
            raiz->fesq = remover_arvore_aux(raiz->fesq, chave, aux, NULL);
            dir = FESQ; // Marcando a direção seguida.
        }
        else if (chave > raiz->item->chave){
            raiz->fdir = remover_arvore_aux(raiz->fdir, chave, aux, NULL);
            dir = FDIR; // Marcando a direção seguida.
        }
        else{
            // Se o nó de chave "chave" tiver dois filhos, procure o seu sucessor (menor da sub-árvore da direita).
            if (raiz->fesq && raiz->fdir){
                *aux = 's';
                // Passando o nó atual (raiz) para que haja troca de conteúdo entre z e v.
                raiz->fdir = remover_arvore_aux(raiz->fdir, chave, aux, raiz);
                dir = FDIR; // Marcando a direção seguida.
            }
            else{
                // printf("Achei o no de remocao %d\n", chave);
                *aux = 'v';
                return raiz;
            }
        }
    }

    // Modo "ache o sucessor".
    if (*aux == 's'){
        if (raiz->fesq){
            // Procurando nó v.
            raiz->fesq = remover_arvore_aux(raiz->fesq, chave, aux, z);
            dir = FESQ;
        }
        else{
            // Trocando o conteúdo de z com o conteúdo de v.
            swap_content(z, raiz);

            // Estou no nó v.
            *aux = 'v';

            return raiz;
        }
    }

    // Modo "operações de balanceamento".
    if (*aux == 'v' || *aux == 'u'){
        p = raiz;

        // Diferenciando se v (ou u) é filho da direita ou da esquerda de p.
        if (dir == FDIR){
            // Faça a linkagem se for remover o nó v. Caso contrário, não preciso de v.
            if (*aux == 'v'){
                v = p->fdir;
                u = v->fdir ? v->fdir : v->fesq;
                p->fdir = u; // Linkando.
                vcolor = v->cor;
                apagar_no(v);
            }
            else{
                u = p->fdir;
                vcolor = PRETO; // Forçando que entre nas operações complexas (u e v pretos).
            }
            
            w = p->fesq;
            *aux = 0;
            
            // Nó v é preto e nó u é preto.
            if (vcolor == PRETO && (!u || u->cor == PRETO)){
                // Caso 1. Nó w é vermelho.
                if (w && w->cor == VERMELHO){
                    p = rot_direita(p);

                    // Marcando a execução do caso 1, pois p não será mais o nó
                    // a ser retornado para a chamada anterior.
                    flag = true;
                    raiz = p;

                    p = p->fdir;
                    w = p->fesq;
                }
                else{
                    flag = false;
                }

                // Nó u é preto e nó w é preto.
                if ((!u || u->cor == PRETO) && w->cor == PRETO){
                    // Caso 2. Os filhos de w são pretos.
                    if ((!w->fesq || w->fesq->cor == PRETO) && (!w->fdir || w->fdir->cor == PRETO)){
                        w->cor = VERMELHO;

                        if (p->cor == VERMELHO){
                            p->cor = PRETO;
                        }
                        else{
                            // O nó p vira o novo Double Black (u).
                            *aux = 'u';
                        }
                    }

                    // Caso 3. Filho da direita de w é vermelho e filho da esquerda de w é preto.
                    if (w->fdir && w->fdir->cor == VERMELHO && (!w->fesq || w->fesq->cor == PRETO)){
                        w = rot_esquerda(w);
                        p->fesq = w;
                    }

                    // Caso 4. Filho da direita de w é X (qualquer cor) e filho da esquerda de w é vermelho.
                    if (w->fesq && w->fesq->cor == VERMELHO){
                        w->fesq->cor = PRETO;
                        vcolor = p->cor;

                        p = rot_direita(p);

                        // Tratando a linkagem.
                        if (flag){
                            raiz->fdir = p;
                        }
                        else{
                            raiz = p;
                        }
                        
                        p->fdir->cor = PRETO;
                        p->cor = vcolor;
                    }
                }
            }
            else{
                // Caso u seja vermelho ou v seja vermelho, marque u como preto.
                if (u){
                    u->cor = PRETO;
                }
            }
        }
        else{ // Operações análogas para a esquerda.
            if (*aux == 'v'){
                v = p->fesq;
                u = v->fdir ? v->fdir : v->fesq;
                p->fesq = u;
                vcolor = v->cor;
                apagar_no(v);
            }
            else{
                u = p->fesq;
                vcolor = PRETO;
            }

            w = p->fdir;
            *aux = 0;
            
            if (vcolor == PRETO && (!u || u->cor == PRETO)){
                // Caso 1. Nó w é vermelho.
                if (w && w->cor == VERMELHO){
                    p = rot_esquerda(p);

                    raiz = p;
                    flag = true;

                    p = p->fesq;
                    w = p->fdir;
                }
                else{
                    flag = false;
                }

                // Nó u é preto e nó w é preto.
                if ((!u || u->cor == PRETO) && w->cor == PRETO){
                    // Caso 2. Os filhos de w são pretos.
                    if ((!w->fesq || w->fesq->cor == PRETO) && (!w->fdir || w->fdir->cor == PRETO)){
                        w->cor = VERMELHO;

                        if (p->cor == VERMELHO){
                            p->cor = PRETO;
                        }
                        else{
                            *aux = 'u';
                        }
                    }

                    // Caso 3. Filho da direita de w é preto e filho da esquerda de w é vermelho.
                    if (w->fesq && w->fesq->cor == VERMELHO && (!w->fdir || w->fdir->cor == PRETO)){
                        w = rot_direita(w);
                        p->fdir = w;
                    }

                    // Caso 4. Filho da direita de w é vermelho e filho da esquerda de w é X (qualquer cor).
                    if (w->fdir && w->fdir->cor == VERMELHO){
                        w->fdir->cor = PRETO;
                        vcolor = p->cor;

                        p = rot_esquerda(p);

                        if (flag){
                            raiz->fesq = p;
                        }
                        else{
                            raiz = p;
                        }
                        
                        p->fesq->cor = PRETO;
                        p->cor = vcolor;
                    }
                }
            }
            else{
                if (u){
                    u->cor = PRETO;
                }
            }
        }
    }

    return raiz;
}

int remover_arvore(ARVORE_BINARIA *arv, int chave){
    char aux = 0;

    if (arv){
        arv->raiz = remover_arvore_aux(arv->raiz, chave, &aux, NULL);

        if (arv->raiz){
            arv->raiz->cor = PRETO;
        }
    }

    return arv != NULL;
}

void print_arvore_aux(NO *raiz){
    printf("{%d", raiz->item->chave);

    if (raiz->fesq){
        printf(",");
        print_arvore_aux(raiz->fesq);
    }

    if (raiz->fdir){
        printf(",");
        print_arvore_aux(raiz->fdir);
    }
    
    printf("}");
}

void print_arvore(ARVORE_BINARIA *arv){
    if (arv){
        if (arv->raiz){
    	   print_arvore_aux(arv->raiz);
        }

        printf("\n");
    }
}
