#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Essa função apenas limpa o buffer da stdin
void cleanBuff()
{
    while (getchar() != '\n')
    {
        continue;
    }
}

// Obtém a quantidade de digitos de um numero
int getQtdeDigts(unsigned int num)
{
    int qtde = 1;
    while (num > 9)
    {
        qtde++;
        num /= 10;
    }
    return qtde;
}

// Essa função lê e valida um inteiro positivo
void readUnsInteger(unsigned int *num)
{
    while (1)
    {
        if (scanf("%u", num) == 1 && (int)*num >= 0)
            break;

        cleanBuff();
        printf("Valor invalido!\nInsira outro: ");
    }

    cleanBuff();
}

// Essa função lê e valida um float positivo
void readFloat(float *preco)
{
    while (1)
    {
        if (scanf("%f", preco) == 1 && *preco >= 0)
            break;

        cleanBuff();
        printf("Preco invalido!\nInsira outro: ");
    }

    cleanBuff();
}

// Retorna a resposta do usuário em perguntas de sim e nao
int simNaoPrompt()
{
    char c;
    while (1)
    {
        c = getchar();
        cleanBuff();

        switch (c)
        {
        case 's':
        case 'S':
            return 1;

        case 'n':
        case 'N':
            return 0;

        default:
            printf("\nResposta invalida!\nPor favor, tente de novo: ");
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    AVLNodo *mainTree = NULL, *el;

    int maiorQtdeDigs = 0, choice;

    unsigned int usrKey;
    float preco;
    char nomeProd[TAM_PROD_STR], format[10];

    // Cria a string que será passada para o scanf de leitura do nome do produto
    snprintf(format, sizeof(format), "%%%d[^\n]s", TAM_PROD_STR - 1);

    while (1)
    {
        printf(
            "1- Inserir\n"
            "2- Listar Vertical\n"
            "3- Listar chaves em formato de arvore\n"
            "4- Procurar por chave\n"
            "5- Editar elemento\n"
            "6- Excluir elemento\n"
            "7- Imprimir elementos ordenados\n"
            "8- Quantidade de elementos\n"
            "9- Altura da arvore\n" //
            "0- Sair\n"
            "\nEscolha: ");

        readUnsInteger(&choice);
        printf("\n");

        switch (choice)
        {
        // Inserir
        case 1:
            printf("Chave: ");
            readUnsInteger(&usrKey);
            if (usrKey == 0)
            {
                printf("Valor invalido!\n");
                getchar();
                break;
            }

            printf("Preco do produto: ");
            readFloat(&preco);

            printf("Nome do produto: ");
            scanf(format, nomeProd);
            cleanBuff();

            // Atualiza o valor da maior quantidade de dígitos nas chaves da árvore
            int aux = getQtdeDigts(usrKey);
            maiorQtdeDigs = (aux > maiorQtdeDigs) ? aux : maiorQtdeDigs;

            // printf("\nQtde digitos: %d\n", maiorQtdeDigs);

            if (incluir(&mainTree, usrKey, preco, nomeProd))
                printf("\nFeito!\n");
            getchar();
            break;

        // Listagem vertical
        case 2:
            imprimeArvVertical(mainTree);
            getchar();
            break;

        // Listagem de arvore
        case 3:
            imprimePorNivel(mainTree, maiorQtdeDigs);
            getchar();
            break;

        // Procurar chave
        case 4:
            printf("Digite uma chave para buscar: ");
            readUnsInteger(&usrKey);

            el = buscarEl(mainTree, usrKey);
            printf("\n");

            if (el)
                imprimeEl(el);
            else
                printf("Elemento nao encontrado.\n");

            getchar();
            break;

        // Editar valor de um elemento
        case 5:
            printf("Informe a chave do elemento que deseja editar: ");
            readUnsInteger(&usrKey);

            // Remove o elemento a ser editado
            el = removeNodo(&mainTree, usrKey);

            if (el)
            {
                printf("\n");

                // Exibe para o usuário o elemento que ele está editando
                imprimeEl(el);

                // Salva as informações do elemento
                usrKey = getElKey(el);
                preco = getElPrice(el);
                strncpy(nomeProd, getElName(el), TAM_PROD_STR * sizeof(char));

                // Apaga o elemento da memoria
                free(el);

                // Nova chave
                printf("\nDeseja alterar a chave? s/n\n");
                if (simNaoPrompt())
                {
                    while (1)
                    {
                        printf("Chave: ");
                        readUnsInteger(&usrKey);
                        if (usrKey == 0)
                        {
                            printf("Valor invalido! Tente novamente.\n");
                            getchar();
                            continue;
                        }
                        else
                        {
                            if (buscarEl(mainTree, usrKey))
                            {
                                printf("\nEsta chave ja existe! Insira outra.\n\n");
                                continue;
                            }
                            break;
                        }
                    }

                    // Atualiza o valor da maior quantidade de dígitos nas chaves da árvore
                    int aux = getQtdeDigts(usrKey);
                    maiorQtdeDigs = (aux > maiorQtdeDigs) ? aux : maiorQtdeDigs;

                    // printf("\nQtde digitos: %d\n", maiorQtdeDigs);
                }

                // Novo preco
                printf("Deseja alterar o preco? s/n\n");
                if (simNaoPrompt())
                {
                    printf("Preco do produto: ");
                    readFloat(&preco);
                }

                // Novo nome
                printf("Deseja alterar o nome do produto? s/n\n");
                if (simNaoPrompt())
                {
                    printf("Nome do produto: ");
                    scanf(format, nomeProd);
                    cleanBuff();
                }

                // Insere o elemento editado
                if (incluir(&mainTree, usrKey, preco, nomeProd))
                    printf("\nFeito!\n");
            }
            else
                printf("\nO elemento nao existe.\n");

            getchar();
            break;

        // Excluir
        case 6:
            printf("Digite uma chave para excluir: ");
            readUnsInteger(&usrKey);

            el = removeNodo(&mainTree, usrKey);

            if (el)
            {
                printf("\nElemento removido: \n");
                imprimeEl(el);
                free(el);
            }

            getchar();
            break;

        // Imprime ordenado
        case 7:
            imprimeOrdenado(mainTree);
            printf("\n");

            getchar();
            break;

        // Num. elementos na árvore
        case 8:
            printf("Quantidade de elementos na arvore: %d\n", qtdeElementos(mainTree));

            getchar();
            break;

        // Altura da árvore
        case 9:
            printf("Altura da arvore: %d\n", alturaArv(mainTree));

            getchar();
            break;

        case 0:
            freeTree(mainTree);
            exit(EXIT_SUCCESS);

        default:
            break;
        }

        system("cls");
    }

    return 0;
}