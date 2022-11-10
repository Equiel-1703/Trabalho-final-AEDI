#define TAM_PROD_STR 31

typedef struct _avlNode AVLNodo;

// Inserir elemento
int incluir(AVLNodo **tree, unsigned int chave, float preco, char *prod);

// Remove um nodo da árvore e o retorna
AVLNodo *removeNodo(AVLNodo **raiz, int key);

// Retorna a altura de uma árvore passada por parâmetro
int alturaArv(AVLNodo *tree);

// Busca e retorna um elemento na árvore usando a chave
AVLNodo *buscarEl(AVLNodo *tree, unsigned int key);

// Retorna a quantidade de elementos numa árvore
int qtdeElementos(AVLNodo *tree);

// Desaloca a árvore da memória
void freeTree(AVLNodo *tree);

// Verifica arvore vazia
int estaVazia(AVLNodo *tree);

// Funções de impressão
void imprimeEl(AVLNodo *el);
void imprimePorNivel(AVLNodo *root, int noOfDigits);
void imprimeArvVertical(AVLNodo *nodo);
void imprimeOrdenado(AVLNodo *tree);

// Getters
unsigned int getElKey(AVLNodo *el);
float getElPrice(AVLNodo *el);
char *getElName(AVLNodo *el);
