#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//No com as informacoes do produto, fator de balanceamento e enderecos para o pai, no da esquerda e da direita
struct noAVL
{
	int altura; //altura da arvore
	int codigo; //codigo do produto
	char nome[100]; //nome do produto
	int quantidade; //quantidade em estoque
	double custo; //custo unitario para produzir o produto
	double preco; //custo unitario de venda do produto
	int fb; //fator de balanceamento 
	struct noAVL *pai; //endereco para o no pai
	struct noAVL *esquerda; //endereco para o filho da esquerda
	struct noAVL *direita; //endereco para o filho da direita
};

//inicia os ponteiros para NULL
void inicia(struct noAVL *no)
{
	no->fb = -10; //Para identificar que esra vazio
	no->pai = NULL;
	no->esquerda = NULL;
	no->direita = NULL;
}

// Devolve a altura do no
int pegaraltura(struct noAVL *no)
{
    if (no == NULL)
        return 0;
	
    return no->altura;
}

// Devolve fator de balanceamento do no
int pegarfb(struct noAVL *no)
{
    if (no == NULL)
        return 0;
	
    return (pegaraltura(no->esquerda) - pegaraltura(no->direita));
}

//Devolve o maior valor
int pegamaior(struct noAVL *esquerda, struct noAVL *direita)
{
	int esq, dir;
	
	if(esquerda != NULL)
		esq = esquerda->altura;
	else
		esq = 0;
	
	if(direita != NULL)
		dir = direita->altura;
	else
		dir = 0;
	
	if(esq > dir)
		return esq;
	else
		return dir;
}

//Rotacao RR
struct noAVL* RR(struct noAVL *no)
{
	printf("entrou RR\n");
	struct noAVL *auxiliar_filho = no->esquerda;
	struct noAVL *auxiliar_pai = no->pai;
	
	//Se nao for raiz
	if(auxiliar_pai != NULL)
	{		
		auxiliar_filho->pai = auxiliar_pai;
		auxiliar_pai->esquerda = auxiliar_filho;
		no->pai = auxiliar_filho;
		
		no->esquerda = auxiliar_filho->direita; 
		auxiliar_filho->direita = no;
		
		//Atualizar o fb do pai
		auxiliar_pai->fb = pegarfb(auxiliar_pai);
		

	}else //Se for raiz
	{
		printf("É raiz\n");
		auxiliar_filho->pai = no->pai;
		no->pai = auxiliar_filho;
		
		no->esquerda = auxiliar_filho->direita;
		auxiliar_filho->direita = no;
		
	}
	printf("rodou\n");
	//Ajeita as alturas dos nos
	no->altura = pegamaior(no->esquerda, no->direita)+1;
	auxiliar_filho->altura = pegamaior(auxiliar_filho->esquerda, auxiliar_filho->direita)+1; 	
	printf("ajeitou altura\n");
	auxiliar_filho->fb = pegarfb(auxiliar_filho);
	no->fb = pegarfb(no);
	printf("ajeitou fb\n");
	return auxiliar_filho;
}

//Rotacao LL
struct noAVL* LL(struct noAVL *no)
{
	printf("entrou LL\n");
	struct noAVL *auxiliar_filho = no->direita;
	struct noAVL *auxiliar_pai = no->pai;
	
	//Se nao for raiz
	if(auxiliar_pai != NULL)
	{		
		auxiliar_filho->pai = auxiliar_pai;
		auxiliar_pai->direita = auxiliar_filho;
		no->pai = auxiliar_filho;
		
		no->direita = auxiliar_filho->esquerda; 
		auxiliar_filho->esquerda = no;
		
		//Atualizar o fb do pai
		auxiliar_pai->fb = pegarfb(auxiliar_pai);
		

	}else //Se for raiz
	{
		printf("É raiz\n");
		auxiliar_filho->pai = no->pai;
		no->pai = auxiliar_filho;
		
		no->direita = auxiliar_filho->esquerda;
		auxiliar_filho->esquerda = no;
		
	}
	printf("rodou\n");
	//Ajeita as alturas dos nos
	no->altura = pegamaior(no->esquerda, no->direita)+1;
	auxiliar_filho->altura = pegamaior(auxiliar_filho->esquerda, auxiliar_filho->direita)+1; 	
	printf("ajeitou altura\n");
	auxiliar_filho->fb = pegarfb(auxiliar_filho);
	no->fb = pegarfb(no);
	printf("ajeitou fb\n");
	return auxiliar_filho;
}

//Rotacao RL
struct noAVL* RL(struct noAVL *no)
{
	no->direita = RR(no->direita);
	no = LL(no);
	
	return no;
}

//Rotacao LR
struct noAVL* LR(struct noAVL *no)
{
	no->esquerda = LL(no->esquerda);
	no = RR(no);
	
	return no;
}

//Balanceia os nos a partir do no recebido ate a raiz, verificando os fb e ajeitando-os
struct noAVL* balancear(struct noAVL* no)
{
	//Em cada no ele ajeita a altura, ajeita o fb e verifica se precisa fazer alguma rotacao
	while(no->pai != NULL)
	{
		no->altura = pegamaior(no->esquerda, no->direita)+1;
		no->fb = pegarfb(no);
			
		//So precisa fazer LL aqui, pois so ocorre insercao na direita, pois os codigos sao sempres maiores que os anteriores
		if(no->fb == 2)
		{
			if(no->esquerda->fb == -1)
				no = LL(no->esquerda);
			
			no = LL(no);
			
		}else if(no->fb == -2)
		{
			if(no->direita->fb == 1)
				no = RR(no->direita);
			
			no =  LL(no);
		}
			
		no = no->pai;	
	}
	//Quando sai do while so falta a raiz
	no->altura = pegamaior(no->esquerda, no->direita)+1;
	no->fb = pegarfb(no);
		
	//So precisa fazer LL aqui, pois so ocorre insercao na direita, pois os codigos sao sempres maiores que os anteriores
	if(no->fb == 2)
	{
		if(no->esquerda->fb == -1)
			no = LL(no->esquerda);
		
		no = LL(no);
		
	}else if(no->fb == -2)
	{
		if(no->direita->fb == 1)
			no = RR(no->direita);
		
		no =  LL(no);
	}
	
	return no;
}

//Funcao que insere um novo produto na arvore
struct noAVL* inserir(struct noAVL *no, int codigo)
{
	//Veifica se e o primeiro no a ser inserido ou nao
	if(no->fb == -10)
	{
		printf("Nome do novo produto a ser adicionado:\n");
		scanf(" %99[^\n]", no->nome);
		
		printf("Valor de custo do produto:\n");
		scanf("%lf", &no->custo);
		
		printf("Preco que o produto sera vendido:\n");
		scanf("%lf", &no->preco);
		
		no->codigo = codigo+1;
		no->quantidade = 0;
		no->fb = 0;
		no->altura = 1;
		
		printf("Adicionado com sucesso!\n\n");
		return no;
		
	}else
	{	
	//Cria o novo no
	struct noAVL *novo = (struct noAVL*)malloc(sizeof(struct noAVL));
	
	
		printf("Nome do novo produto a ser adicionado:\n");
		scanf(" %99[^\n]", novo->nome);
		
		printf("Valor de custo do produto:\n");
		scanf("%lf", &novo->custo);
		
		printf("Preco que o produto sera vendido:\n");
		scanf("%lf", &novo->preco);
		
		novo->codigo = codigo+1;
		novo->quantidade = 0;
		novo->fb = 0;
		novo->altura = 1;
		novo->esquerda = NULL;
		novo->direita = NULL;
		
		struct noAVL* temporario = no;
		
		printf("ANTES WHILE :cod: %d fb: %d altura:%d \n", temporario->codigo, temporario->fb, temporario->altura);
		
		//Como o codigo novo sempre sera maior que todos os anteriores, o no sempre sera adicionado na folha mais a direita disponivel
		while(temporario->direita != NULL)
		{
			printf("cod: %d fb: %d altura:%d \n", temporario->codigo, temporario->fb, temporario->altura);
			temporario = temporario->direita;
		}
		printf("DEPOIS WHILE :cod: %d fb: %d altura:%d \n", temporario->codigo, temporario->fb, temporario->altura);
		
		//Agora no tem o pai de novo e rece +1 de altura
		temporario->direita = novo;
		novo->pai = temporario;
		//temporario->altura++;
		
		printf("ANTES WHILE 2 :cod: %d fb: %d altura:%d \n", temporario->codigo, temporario->fb, temporario->altura);
		printf("ANTES WHILE 2 :cod: %d fb: %d altura:%d \n", novo->codigo, novo->fb, novo->altura);
		
		//Depois de adicionado, vamos verificar se a arvore ficou desbalanceada, enquanto ajeita o fator de balanceamento, nesse caso em especial, por so adicionar numeros maiores
		//so pode ocorrer LL e na insercao so pode ocorrer uma rotacao, vemos ate o ultimo filho antes da raiz, para depois comparar com a raiz e retorna-la a main
		while(temporario->pai != NULL)
		{			
			//ajeita a altura e o fator de balanceamento
			temporario->altura = pegamaior(temporario->esquerda, temporario->direita)+1;
			temporario->fb = pegarfb(temporario);
			printf("cod: %d fb: %d\n", temporario->codigo, temporario->fb);
			
			//So precisa fazer LL aqui, pois so ocorre insercao na direita, pois os codigos sao sempres maiores que os anteriores
			if(temporario->fb == -2)
			{
				temporario = LL(temporario);
			}
			
			temporario = temporario->pai;
		}		
		printf("SAIU DO while\n");	
		
		//ajeita a altura e o fator de balanceamento
		temporario->altura = pegamaior(temporario->esquerda, temporario->direita)+1;
		temporario->fb = pegarfb(temporario);
		
		printf("FORA WHILE cod: %d fb: %d altura:%d\n", temporario->codigo, temporario->fb, temporario->altura);
		if(temporario->fb == -2)
			{
				temporario = LL(temporario);
			}

			printf("Adicionado com sucesso!\n\n");
			
			return temporario;
	}
}

//Procura o no na arvore, caso exista, retorna ele, caso negativo retorna NULL
struct noAVL* procurar(struct noAVL *no, int buscando)
{
	if(no->codigo == buscando)
	{
		return no;
	}else if(no != NULL)
	{
		if((no->codigo > buscando) && (no->esquerda != NULL))
		{
			return procurar(no->esquerda,buscando);
		}else if((no->codigo < buscando) && (no->direita != NULL))
		{
			return procurar(no->direita,buscando);
		}
	}
	
	return NULL;
}

//Recebe o filho da esquerda  de um no e vai descendo a arvore toda para a direita ate que seja um no folha
struct noAVL* procuramenor(struct noAVL *no)
{
	while(no->direita != NULL)
	{
		no = no->direita;
	}
	
	return no;
}

//Caso a remocao seja um sucesso, retorna 1, senao retorna 0
struct noAVL* remover(struct noAVL *no)
{
	//Verifica se a raiz é NULL,não existe
	if(no == NULL)
	{
		printf("ERRO: Não existem produtos cadastrados\n\n");
		return NULL;
	}
	
	//codigo do produto que vai ser apagado
	int codigo;
	
	printf("Codigo do produto que deseja excluir:\n\n");
	scanf("%d", &codigo);
	
	//Verifica se existe um no com o codigo
	struct noAVL* alvo = procurar(no, codigo);
	
	//Caso nao exista printa erro e sai da funcao
	if(alvo == NULL)
	{
		printf("ERRO: Não foi possivel apagar o produto, pois ele não existe\n\n");
		return NULL;
	}
	//No para ajudar na delecao, sendo o pai de alvo
	struct noAVL* temporario = alvo->pai;
	
	//Se alvo for o proprio no
	if(alvo == no)
	{
		//Se nao tem filho coloca o fb para -10, para identificar que é a primeira insercao, caso deseja inserir, na funcao de insercao
		if((no->esquerda == NULL) && (no->direita == NULL))//Nao tem filhos
		{
			no->fb = -10;
			
			printf("Removido com sucesso!\n\n");
			return  no;
		}
		//Se so tem filho da direita
		if(no->esquerda == NULL)//Tem so o filho da direita
		{
			no = no->direita;
			
			//balancear a raiz
			no = balancear(no);
			
			printf("Removido com sucesso!\n\n");
			return no;
		}
		//Se so tem filho da esquerda
		if(no->esquerda == NULL)//Tem so o filho da direita
		{
			no = no->esquerda;
			
			//balancear a raiz
			no = balancear(no);
			
			printf("Removido com sucesso!\n\n");
			return no;
		}
		
		//Se tem os 2 filhos
		
		//Usar temporario para receber o menor anterior
		temporario = procuramenor(alvo->esquerda);
		
		//Copiar dados para a raiz
		no->codigo = temporario->codigo;
		strcpy(no->nome, temporario->nome);
		no->quantidade = temporario->quantidade;
		no->custo = temporario->custo;
		no->preco = temporario->preco;
		
		//alvo vai receber o avo do filho do menor anterior, caso tenha filho é util no proximo if, se nao, vai ser usado para ajeitar o fb do resto da arvore
		alvo = temporario->pai;
		
		//Se o menor anterior tiver um filho na esquerda, entao esse filho sera filho de seu avo
		if(temporario->esquerda !=NULL)
		{		
			//Filho da direita de alvo sera o filho da esquerda de temporario
			alvo->direita = temporario->esquerda;
		}
		
		//no vai receber a arvore balanceada e rotacionada, caso necessario
		no = balancear(alvo);
		printf("Removido com sucesso!\n\n");
		return no;
	}
	
	//Verifica se o no é folha, se tem 1 filho ou 2 filhos e entao decidir o que fazer
	if((alvo->esquerda == NULL) && (alvo->direita == NULL))//Caso seja no folha
	{	
		//Caso o codigo de temporario seja maior que o codigo de alvo, alvo é o filho da direita, se nao é o filho da esquerda
		if(temporario->codigo > alvo->codigo)
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->esquerda = NULL;
			free(alvo);
		}else
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->direita = NULL;
			free(alvo);
		}
		//balancear no
		no = balancear(temporario);
		
		printf("Removido com sucesso!\n\n");
		return no;
		
	}else if(alvo->esquerda == NULL)//Tem so o filho da direita
	{
		//Caso o codigo de temporario seja maior que o codigo de alvo, alvo é o filho da direita, se nao é o filho da esquerda
		if(temporario->codigo > alvo->codigo)
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->esquerda = alvo->direita;
			free(alvo);
		}else
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->direita = alvo->direita;
			free(alvo);
		}
		//balancear no
		no = balancear(temporario);
		
		printf("Removido com sucesso!\n\n");
		return no;
		
	}else if(alvo->direita == NULL)//Tem so o filho da esquerda
	{
		//Caso o codigo de temporario seja maior que o codigo de alvo, alvo é o filho da direita, se nao é o filho da esquerda
		if(temporario->codigo > alvo->codigo)
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->esquerda = alvo->esquerda;
			free(alvo);
		}else
		{
			//Coloca o ponteiro para o alvo como nule e apaga ele
			temporario->direita = alvo->esquerda;
			free(alvo);
		}
		
		//balancear no
		no = balancear(temporario);
		
		printf("Removido com sucesso!\n\n");
		return no;
		
	}else //Se alvo tiver os 2 filhos
	{
		//Filho da esquerda do alvo
		struct noAVL* esquerdinha = alvo->esquerda;
		
		//Filho da direita do alvo
		struct noAVL* direitinha = alvo->direita;
		
		//Caso um dos filhos nao tenha filho, substituir por esse no, se nao, substituie pelo seu antecessor imediato
		if((esquerdinha->esquerda == NULL) && (esquerdinha->direita == NULL)) //O filho da esquerda nao tem filhos
		{
			//Caso esquerdinha nao tenha filhos, recebe direitinha na sua direita e substitui alvo
			esquerdinha->direita = direitinha;
			
			//Caso o codigo de temporario seja maior que o codigo de alvo, alvo é o filho da direita, se nao é o filho da esquerda
			if(temporario->codigo > alvo->codigo)
			{
				//Coloca o ponteiro para o alvo como seu filho da esquerda
				temporario->esquerda = alvo->esquerda;
				free(alvo);
			}else
			{
				//Coloca o ponteiro para o alvo como seu filho da direita
				temporario->direita = alvo->esquerda;
				free(alvo);
			}
		
		}else if((direitinha->esquerda == NULL) && (direitinha->direita == NULL)) //O filho da direita nao tem filhos
		{
			//Caso direitinha nao tenha filhos, recebe esquerdinha na sua esquerda e substitui alvo
			direitinha->esquerda = esquerdinha;
			
			//Caso o codigo de temporario seja maior que o codigo de alvo, alvo é o filho da direita, se nao é o filho da esquerda
			if(temporario->codigo > alvo->codigo)
			{
				//Coloca o ponteiro para o alvo como seu filho da esquerda
				temporario->esquerda = alvo->esquerda;
				free(alvo);
			}else
			{
				//Coloca o ponteiro para o alvo como seu filho da direita
				temporario->direita = alvo->esquerda;
				free(alvo);
			}
		}else //Os dois filhos tem filhos
		{
			//Procurar o menor anterior
			struct noAVL* menorAnterior = procuramenor(alvo->esquerda);
			
			//Copiar os dados do menor anterior para o alvo
			alvo->codigo = menorAnterior->codigo;
			strcpy(alvo->nome, menorAnterior->nome);
			alvo->quantidade = menorAnterior->quantidade;
			alvo->custo = menorAnterior->custo;
			alvo->preco = menorAnterior->preco;
			
			//Se o menorAnterior tiver um filho na esquerda, entao, ele entra no lugar do menorAnterior
			if(menorAnterior->esquerda != NULL)
			{
				menorAnterior = menorAnterior->esquerda;
			}
			
			//balanceia do pai do antigo menor anterior menor anterior em diante
			no = balancear(menorAnterior->pai);
		
		}
	}
	printf("Removido com sucesso!\n\n");
	return no;
}


void buscar(struct noAVL *no)
{
	//codigo do produto a ser buscado
	int codigo;
	
	printf("Codigo do produto que deseja exibir informacoes:\n");
	scanf("%d", &codigo);
	
	//Verifica se existe um no com o codigo
	struct noAVL* alvo = procurar(no, codigo);
	
	if(alvo == NULL)
	{
		printf("ERRO:Produto inexistente\n");
	}else
	{
		printf("Codigo: %d\n", alvo->codigo);
		printf("Produto: %s\n", alvo->nome);
		printf("Quantidade em estoque: %d\n", alvo->quantidade);
		printf("Preco de custo unitario: R$ %.2lf\n", alvo->custo);
		printf("Preco de venda unitario: R$ %.2lf\n", alvo->preco);
		printf("\n");
	}
}

//Recebe o codigo e a quantidade de produtos adicionados, caso nao encontre o codigo, retorna 0, caso encontre retorna valor adicionado
int entrada(struct noAVL *no)
{
	//codigo do produto 
	int codigo;
	
	printf("Codigo do produto que deseja adicionar ao estoque:\n");
	scanf("%d", &codigo);
	
	//Verifica se existe um no com o codigo
	struct noAVL* temporario = procurar(no, codigo);
	
	if(temporario == NULL)
	{
		printf("ERRO: Não foi possivel encontrar produto com codigo %d\n\n", codigo);
		return 0;
	}else
	{
		//quantidade a ser adicionado
		int quantidade;
		
		printf("Quantidade de produtos que serao adicionados:\n");
		scanf("%d", &quantidade);
		
		temporario->quantidade += quantidade;
		
		return quantidade;
	}
}

//Recebe o codigo e a quantidade de produtos que serao retirados, caso nao encontre o codigo ou o tamanho seja maior que o possivel, retorna 0, caso encontre retorna valor retirado
double *saida(struct noAVL *no)
{
	//codigo do produto 
	int codigo;
	
	//Vetor com as informacoes de saida
	double *infos = (double*)malloc(sizeof(double)*3);
	
	printf("Codigo do produto que deseja retirar do estoque:\n");
	scanf("%d", &codigo);
	
	//Verifica se existe um no com o codigo
	struct noAVL* temporario = procurar(no, codigo);
	
	if(temporario == NULL)
	{
		printf("ERRO: Não foi possivel encontrar produto com codigo %d\n\n", codigo);
		return NULL;
	}else
	{
		//quantidade a ser retirada
		int quantidade;
		
		printf("Quantidade de produtos que serao retirados:\n");
		scanf("%d", &quantidade);
		
		if(quantidade > temporario->quantidade)
		{
			printf("ERRO: Quantidade maior do que a existente no estoque\n\n");
			
			return NULL;
		}
		
		temporario->quantidade -= quantidade;
		
		infos[0] = quantidade;
		infos[1] = temporario->custo * quantidade;
		infos[2] = temporario->preco * quantidade;
		
		return infos;
	}
}

//Printa varias informacoes sobre as vendas
void relatorio (int prod_in, int prod_out, int entraram, double *sairam)
{
	//Quantidades de produtos diferentes em estoque
	printf("Total de produtos diferentes em em estoque: %d\n", (prod_in - prod_out));
	
	//Quantidade de produtos em estoque = produtos que entraram - os que sairam do estoque
	printf("Total de produtos em estoque: %d\n", (entraram - (int)sairam[0]));
	
	//Quantidade de itens que ja entrou em estoque
	printf("Total de produtos que ja entraram no estoque: %d\n", entraram);
	
	//Quantidade de itens que ja saiu de estoque
	printf("Total de produtos que ja sairam do estoque: %d\n", (int)sairam[0]);
	
	//R$ total de vendas
	printf("Total de dinheiro recebido em vendas: R$ %.2lf\n", sairam[2]);
	
	//Custo total dos produtos vendidos
	printf("Custo total dos produtos vendidos: R$ %.2lf\n", sairam[1]);
	
	//Lucro total da venda de produtos
	printf("Lucro total das vendas: R$ %.2lf\n", (sairam[1] - sairam[2]));
	
	//%de lucro
	printf("Lucro em porcentagem: %.2lf%%\n", ((sairam[2]/(sairam[1]))*100);
}

int main()
{
	//operacao vai ser a operacao escolhida no while e o controle vai controlar se contunua ou sai do programa
	int operacao, controle = 1;
	
	//codigo inicia como 0 e aumenta quando se insere novos produtos
	int codigo = 0;
	
	//Quantidade de produtos que foram cadastrados e retirados da arvore
	int prod_in = 0, prod_out = 0;
	
	//Quantidade de produtos que entraram em estoque 
	int entraram = 0;
	
	//vetor que guarda a quantidade de produtos que sairam do estoque, custo dos produtos vendidos e lucro dos produtos vendidos
	double sairam[3];
	
	sairam[0] = 0;
	sairam[1] = 0;
	sairam[2] = 0;
	
	//Vetor para receber os valores que sairam da funcao saida
	double *recebe;
	
	//Cria o no
	struct noAVL *no = (struct noAVL*)malloc(sizeof(struct noAVL));
	
	inicia(no);
	
	//Cria o no para ser usado em remover
	struct noAVL *foi;

	//Menu do programa
	while(controle)
	  {
		printf("Escolha uma das seguintes funcoes:\n");
		printf("Fechar programa: 0\n");
		printf("Inserir novo produto: 1\n");
		printf("Remover produto: 2\n");
		printf("Buscar produto: 3\n");
		printf("Entrada de produto: 4\n");
		printf("Saida de produto: 5\n");
		printf("Relatorio: 6\n");
		scanf("%d", &operacao);
		switch(operacao)
		{
			case 0:
					controle = 0;
					break;
			case 1:
					//insere um novo produto e incrementa +1 para o codigo e +1 para prod_in
					no = inserir(no, codigo);
					prod_in++;
					codigo++;
					break;
			case 2:
					//Tenta remover um produto da arvore, caso tenha sucesso retorna a raiz, se nao, retorna NULL
					foi = remover(no);
					
					//Se foi for NULL, nao faz nada, se nao for NULL, prod_out recebe +1 e no(raiz) recebe foi(possivel nova raiz)
					if(foi != NULL)
					{
						prod_out++;
						
						no = foi;
					}
					break;
			case 3:
					//Insere o codigo e retorna os dados do produto
					buscar(no);
					break;
			case 4:
					//Adiciona uma quantidade de produtos em um no com determinado codigo
					entraram += entrada(no);
					break;
			case 5:
					//Retira uma quantidade de produtos em um no com determinado codigo
					recebe = saida(no);
					
					if(recebe!= NULL)
					{
						sairam[0] += recebe[0];
						sairam[1] += recebe[1];
						sairam[2] += recebe[2];
					}
					
					break;
			case 6:
					//Mostra alguns dados sobre as vendas
					relatorio(prod_in, prod_out, entraram, sairam);
					break;
		  default:
				  printf("Erro! Use um número válido\n");
				  break;
		}
	  }
	
	
}