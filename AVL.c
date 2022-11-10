#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "AVL.h"

struct _avlNode
{
    unsigned int key;
    float preco;
    char produto[TAM_PROD_STR];

    struct _avlNode *lChild;
    struct _avlNode *rChild;
};

// Cria e inicializa um nó da árvore
AVLNodo *criarNodo(unsigned int chave, float preco, char *prod)
{
    AVLNodo *newNode = (AVLNodo *)malloc(sizeof(AVLNodo));

    if (newNode)
    {
        newNode->key = chave;
        newNode->preco = preco;
        strncpy(newNode->produto, prod, TAM_PROD_STR * sizeof(char));

        newNode->lChild = NULL;
        newNode->rChild = NULL;

        return newNode;
    }

    printf("\nErro de alocacao de memoria!\n\n");
    getchar();
    exit(EXIT_FAILURE);
}

// --------------------- Rotações & Balanceamento ---------------------
void LLrotation(AVLNodo **A)
{
    AVLNodo *B = (*A)->lChild;

    // Desce o A e sobe o B
    (*A)->lChild = B->rChild;
    B->rChild = *A;

    // Atualizando raíz
    *A = B;
}
void LRrotation(AVLNodo **A)
{
    AVLNodo *B = (*A)->lChild;
    AVLNodo *C = B->rChild;

    // Mudando filhos do C
    (*A)->lChild = C->rChild;
    B->rChild = C->lChild;

    // Subindo o C
    C->lChild = B;
    C->rChild = *A;

    // Atualizando raíz
    *A = C;
}
void RRrotation(AVLNodo **A)
{
    AVLNodo *B = (*A)->rChild;

    // Desce o A e sobe o B
    (*A)->rChild = B->lChild;
    B->lChild = *A;

    // Atualizando raíz
    *A = B;
}
void RLrotation(AVLNodo **A)
{
    AVLNodo *B = (*A)->rChild;
    AVLNodo *C = B->lChild;

    // Mudando filhos do C
    (*A)->rChild = C->lChild;
    B->lChild = C->rChild;

    // Subindo o C
    C->lChild = *A;
    C->rChild = B;

    // Atualizando raíz
    *A = C;
}

// Retorna o fator de balanceamento
int getBalanceFactor(AVLNodo *tree)
{
    return alturaArv(tree->lChild) - alturaArv(tree->rChild);
}

// Função de balanceamento
void balancear(AVLNodo **tree)
{
    int FBRaiz = getBalanceFactor(*tree);

    if (FBRaiz == 2)
    {
        int FBlChild = getBalanceFactor((*tree)->lChild);

        if (FBlChild == 1)
            LLrotation(tree);

        else
            LRrotation(tree);
    }
    else if (FBRaiz == -2)
    {
        int FBrChild = getBalanceFactor((*tree)->rChild);

        if (FBrChild == -1)
            RRrotation(tree);

        else
            RLrotation(tree);
    }
}

// --------------------- Inserção, Remoção e Busca ---------------------

// Retorna o antecedente em ordem simétrica
int inOrderPredKey(AVLNodo *tree)
{
    if (tree == NULL)
        return -1;

    while (tree->rChild)
        tree = tree->rChild;

    return tree->key;
}

// Retirar um elemento da árvore
AVLNodo *removeNodo(AVLNodo **raiz, int key)
{
    if (*raiz == NULL)
        return NULL;

    AVLNodo *temp;

    if ((*raiz)->key == key) // Achamos o nodo a ser apagado
    {
        temp = *raiz; // Salvamos o nó que vai ser removido

        if ((*raiz)->lChild == NULL && (*raiz)->rChild == NULL) // Nó folha
            *raiz = NULL;

        else if ((*raiz)->lChild == NULL || (*raiz)->rChild == NULL) // Não tem um dos filhos
            *raiz = (*raiz)->lChild == NULL ? (*raiz)->rChild : (*raiz)->lChild;

        else
        {
            // Se chegar até aqui é pq o nó a ser apagados tem dois filhos
            // Vamos então pegar o antecessor in-ordem dele (maior nó da sub-árvore à esquerda)
            // e colocar no lugar
            AVLNodo *pred = removeNodo(&((*raiz)->lChild), inOrderPredKey((*raiz)->lChild));

            pred->lChild = (*raiz)->lChild;
            pred->rChild = (*raiz)->rChild;
            *raiz = pred;
        }
    }
    else
    {
        if (key < (*raiz)->key)
            temp = removeNodo((&(*raiz)->lChild), key);
        else
            temp = removeNodo(&((*raiz)->rChild), key);
    }

    if (*raiz)
        balancear(raiz);

    return temp;
}

// Insere um elemento na árvore
int incluir(AVLNodo **tree, unsigned int chave, float preco, char *prod)
{
    // Se a árvore está vazia, insere o nodo na raíz
    if (*tree == NULL)
    {
        AVLNodo *newNode = criarNodo(chave, preco, prod);
        *tree = newNode;

        return 1;
    }

    int ret = 0;

    // Se não for vazio, verifica para qual direção deve inserir
    if (chave < (*tree)->key)
        ret = incluir(&(*tree)->lChild, chave, preco, prod); // Esquerda, chave menor

    else if (chave > (*tree)->key)
        ret = incluir(&(*tree)->rChild, chave, preco, prod); // Direita, chave maior

    else // Esse else só será executado caso o usuário tente inserir um valor repetido
    {
        printf("\nVoce nao pode inserir uma chave repetida!\n");
        return 0;
    }

    // Balanceia a árvore, se necessário
    balancear(tree);

    return ret;
}

// Busca e retorna um elemento na árvore usando a chave
AVLNodo *buscarEl(AVLNodo *tree, unsigned int key)
{
    if (tree == NULL)
        return NULL;

    while (tree && tree->key != key)
        tree = (key < tree->key) ? tree->lChild : tree->rChild;

    return tree;
}

// --------------------- Outras ---------------------

// Verifica se a árvore está vazia
int estaVazia(AVLNodo *tree)
{
    return tree ? 0 : 1;
}

// Função usada para obter a altura de uma árvore
int alturaArv(AVLNodo *tree)
{
    if (tree == NULL)
        return -1;
    else
    {
        int lheight = alturaArv(tree->lChild);
        int rheight = alturaArv(tree->rChild);

        return lheight > rheight ? lheight + 1 : rheight + 1;
    }
}

// Retorna a quantidade de elementos numa árvore
int qtdeElementos(AVLNodo *tree)
{
    if (tree == NULL)
        return 0;

    int qtdeL = qtdeElementos(tree->lChild);
    int qtdeR = qtdeElementos(tree->rChild);

    return qtdeL + qtdeR + 1;
}

// Essa função usa o percurso em pós-ordem para liberar toda a árvore da memória
void freeTree(AVLNodo *tree)
{
    if (tree)
    {
        freeTree(tree->lChild);
        freeTree(tree->rChild);
        free(tree);
    }
}

// --------------------- Funções de impressão ---------------------

// Imprime os dados de um elemento passado por parametro
void imprimeEl(AVLNodo *el)
{
    printf(
        "Key: %u\n"
        "Preco: %.2f\n"
        "Produto: %s\n",
        el->key, el->preco, el->produto);
}

// Imprime um nível da arvore (função usada pela imprimePorNível)
void imprimeNivel(AVLNodo *tree, int nivel, int ident, int noOfDigits)
{
    if (nivel == 1)
    {
        if (tree)
            printf("%*u", noOfDigits, tree->key);

        else
        {
            for (int i = 0; i < noOfDigits; i++)
                printf(" ");
        }

        for (int i = 0; i < ident; i++)
            printf(" ");
    }
    else if (tree && nivel > 1)
    {
        imprimeNivel(tree->lChild, nivel - 1, ident, noOfDigits);
        imprimeNivel(tree->rChild, nivel - 1, ident, noOfDigits);
    }
}

// Imprime a árvore em ordem de nível
void imprimePorNivel(AVLNodo *tree, int noOfDigits)
{
    int h = alturaArv(tree);

    if (h == -1)
    {
        printf("Arvore vazia.\n");
        return;
    }

    int noNodes = pow(2, h + 1) - 1;
    int ident = (noNodes - 1) / 2;
    int prevIdent = ident;

    for (int i = 1; i <= h + 1; i++)
    {
        for (int j = 0; j < ident * noOfDigits; j++)
            printf(" ");

        imprimeNivel(tree, i, prevIdent * noOfDigits, noOfDigits);
        printf("\n");

        prevIdent = ident;
        ident = (ident - 1) / 2;
    }
}

// Função recursiva usada pela função de impressão vertical
void imprimeArvVerticalRec(AVLNodo *nodo, int tab, char c)
{
    for (int i = 0; i < tab; i++)
        printf("-");

    printf(" %c ", c);

    // printf("ADD: %p | Key: %3u | $%1.2f | %s | lChild: %p | rChild: %p\n", nodo, nodo->key, nodo->preco, nodo->produto, nodo->lChild, nodo->rChild);
    printf("Key: %3u | $%1.2f | %s \n", nodo->key, nodo->preco, nodo->produto);

    if (nodo->lChild != NULL)
        imprimeArvVerticalRec(nodo->lChild, tab + 1, 'L');
    if (nodo->rChild != NULL)
        imprimeArvVerticalRec(nodo->rChild, tab + 1, 'R');
}

// Imprime a árvore verticalmente (com estrutura de pasta e subpasta)
void imprimeArvVertical(AVLNodo *nodo)
{
    if (nodo)
    {
        printf("root: ");

        // printf("ADD: %p | Key: %3u | $%1.2f | %s | lChild: %p | rChild: %p\n", nodo, nodo->key, nodo->preco, nodo->produto, nodo->lChild, nodo->rChild);
        printf("Key: %3u | $%1.2f | %s \n", nodo->key, nodo->preco, nodo->produto);

        if (nodo->lChild != NULL)
            imprimeArvVerticalRec(nodo->lChild, 1, 'L');
        if (nodo->rChild != NULL)
            imprimeArvVerticalRec(nodo->rChild, 1, 'R');
    }
    else
        printf("Arvore vazia.\n");
}

// Imprime a arvore de forma ordenada (isso nada mais é que um percurso in-ordem)
void imprimeOrdenado(AVLNodo *tree)
{
    if (tree == NULL)
        return;

    imprimeOrdenado(tree->lChild);
    printf("%d ", tree->key);
    imprimeOrdenado(tree->rChild);
}

// --------------------- Getters ---------------------
unsigned int getElKey(AVLNodo *el)
{
    return el->key;
}

float getElPrice(AVLNodo *el)
{
    return el->preco;
}

char *getElName(AVLNodo *el)
{
    return el->produto;
}