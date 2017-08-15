/* Victor Forbes - 9293394 */

#include <stdio.h>
#include "item.h"
#include "arvore_binaria.h"

int main(int argc, char *argv[]){
	ARVORE_BINARIA *arv = criar_arvore();
	char operacao;
	int chave;

	while (scanf("%c %d\n", &operacao, &chave) != EOF){
		if (operacao == 'I'){
			inserir_arvore(arv, criar_item(chave, chave));
		}
		else if (operacao == 'R'){
			remover_arvore(arv, chave);
		}

		print_arvore(arv);
	}

	apagar_arvore(&arv);

	return 0;
}