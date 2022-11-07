#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// Retirar um elemento da árvore
AVLNodo *removeNodo(AVLNodo **raiz, int key)
{
    AVLNodo *ptr = *raiz, *tailPtr = NULL;

    if (*raiz == NULL)
    {
        printf("\nArvore vazia.\n");
        return NULL;
    }

    // Procura o nodo para remover
    while (ptr && ptr->key != key)
    {
        tailPtr = ptr;

        if (key < ptr->key)
            ptr = ptr->lChild;
        else
            ptr = ptr->rChild;
    }

    if (ptr == NULL) // Não encontrou o nó a ser removido
    {
        printf("\nNodo nao encontrado.\n");
        return NULL;
    }

    if (ptr->lChild == NULL && ptr->rChild == NULL) // Se for uma folha
    {
        if (ptr == *raiz)
            *raiz = NULL;
        else if (ptr->key < tailPtr->key)
            tailPtr->lChild = NULL;
        else
            tailPtr->rChild = NULL;
    }
    else if (ptr->lChild == NULL || ptr->rChild == NULL) // Se não tem um dos filhos
    {
        if (ptr == *raiz)
            *raiz = (ptr->lChild == NULL) ? ptr->rChild : ptr->lChild;
        else if (ptr->key < tailPtr->key)
            tailPtr->lChild = (ptr->lChild == NULL) ? ptr->rChild : ptr->lChild;
        else
            tailPtr->rChild = (ptr->lChild == NULL) ? ptr->rChild : ptr->lChild;
    }
    else // Se tem os dois filhos
    {
        // Obtemos o antecedente in-ordem do nó a ser deletado
        AVLNodo *inPred = ptr->lChild, *tailInPred = NULL;

        while (inPred && inPred->rChild)
        {
            tailInPred = inPred;
            inPred = inPred->rChild;
        }

        if (tailInPred)
        {
            tailInPred->rChild = inPred->lChild;
            inPred->lChild = ptr->lChild;
        }

        inPred->rChild = ptr->rChild;

        if (tailPtr)
        {
            if (ptr->key < tailPtr->key)
                tailPtr->lChild = inPred;
            else
                tailPtr->rChild = inPred;
        }
        else
            *raiz = inPred;
    }

    if (*raiz)
        balancear(raiz);

    return ptr;
}

// Insere um elemento na árvore
int incluir(AVLNodo **tree, unsigned int chave, float preco, char *prod)
{
    AVLNodo *tailPtr = NULL, *ptr = *tree;

    // Depois desse while, tail ptr vai ter o endereço do nó pai onde vamos inserir o novo elemento
    while (ptr)
    {
        tailPtr = ptr;

        if (chave < ptr->key)
            ptr = ptr->lChild;

        else if (chave > ptr->key)
            ptr = ptr->rChild;

        else // A chave é igual, não inserimos
        {
            printf("\nVoce nao pode inserir uma chave repetida!\n");
            return 0;
        }
    }

    AVLNodo *newNode = criarNodo(chave, preco, prod);

    // Se tailPtr for vazio, inserimos o nodo na raíz
    if (tailPtr == NULL)
    {
        *tree = newNode;

        return 1;
    }

    // Se não for vazio, verifica para qual direção deve inserir
    if (chave < tailPtr->key)
        tailPtr->lChild = newNode;

    else
        tailPtr->rChild = newNode;

    // Balanceia a árvore, se necessário
    balancear(tree);

    return 1;
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

// Função usada para obter a altura de uma árvore

/*
A função para achar a altura sem usar recursão exige uma fila.
Sua conversão para iterativa não vale a pena, pois o custo de desenvolvimento
e de recursos computacionais são maiores.

A mesma coisa acontece na função que obtém a quantidade de elementos na árvore e
na função que libera a árvore.
*/
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

    printf("ADD: %p | Key: %3u | $%1.2f | %s | lChild: %p | rChild: %p\n", nodo, nodo->key, nodo->preco, nodo->produto, nodo->lChild, nodo->rChild);
    // printf("Key: %3u | $%1.2f | %s \n", nodo->key, nodo->preco, nodo->produto);

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

        printf("ADD: %p | Key: %3u | $%1.2f | %s | lChild: %p | rChild: %p\n", nodo, nodo->key, nodo->preco, nodo->produto, nodo->lChild, nodo->rChild);
        // printf("Key: %3u | $%1.2f | %s \n", nodo->key, nodo->preco, nodo->produto);

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