#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar um n� da �rvore de Huffman
typedef struct No {
    char caractere;
    int frequencia;
    struct No* esquerdo;
    struct No* direito;
} No;

// Estrutura para armazenar uma pilha de n�s da �rvore de Huffman
typedef struct Pilha {
    int tamanho;
    int capacidade;
    No** array;
} Pilha;

// Fun��o para criar um novo n�
No* novoNo(char caractere, int frequencia) {
    No* temp = (No*) malloc(sizeof(No));
    temp->caractere = caractere;
    temp->frequencia = frequencia;
    temp->esquerdo = temp->direito = NULL;
    return temp;
}

// Fun��o para criar uma pilha com capacidade fornecida
Pilha* criarPilha(int capacidade) {
    Pilha* pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->tamanho = 0;
    pilha->capacidade = capacidade;
    pilha->array = (No**) malloc(pilha->capacidade * sizeof(No*));
    return pilha;
}

// Fun��o para trocar dois n�s
void trocarNo(No** a, No** b) {
    No* temp = *a;
    *a = *b;
    *b = temp;
}

// Fun��o para organizar a pilha de acordo com a frequ�ncia (Min-Heap)
void minHeapify(Pilha* pilha, int idx) {
    int menor = idx;
    int esquerdo = 2 * idx + 1;
    int direito = 2 * idx + 2;

    if (esquerdo < pilha->tamanho && pilha->array[esquerdo]->frequencia < pilha->array[menor]->frequencia)
        menor = esquerdo;

    if (direito < pilha->tamanho && pilha->array[direito]->frequencia < pilha->array[menor]->frequencia)
        menor = direito;

    if (menor != idx) {
        trocarNo(&pilha->array[menor], &pilha->array[idx]);
        minHeapify(pilha, menor);
    }
}

// Fun��o para verificar se a pilha cont�m um �nico n�
int tamanhoUnico(Pilha* pilha) {
    return (pilha->tamanho == 1);
}

// Fun��o para extrair o n� de frequ�ncia m�nima da pilha
No* extrairMinimo(Pilha* pilha) {
    No* temp = pilha->array[0];
    pilha->array[0] = pilha->array[pilha->tamanho - 1];
    --pilha->tamanho;
    minHeapify(pilha, 0);
    return temp;
}

// Fun��o para inserir um novo n� na pilha
void inserirPilha(Pilha* pilha, No* no) {
    ++pilha->tamanho;
    int i = pilha->tamanho - 1;

    while (i && no->frequencia < pilha->array[(i - 1) / 2]->frequencia) {
        pilha->array[i] = pilha->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    pilha->array[i] = no;
}

// Fun��o para criar e construir a pilha de n�s
Pilha* criarConstruirPilha(char caracteres[], int frequencias[], int tamanho) {
    Pilha* pilha = criarPilha(tamanho);

    for (int i = 0; i < tamanho; ++i)
        pilha->array[i] = novoNo(caracteres[i], frequencias[i]);

    pilha->tamanho = tamanho;
    for (int i = (pilha->tamanho - 1) / 2; i >= 0; --i)
        minHeapify(pilha, i);

    return pilha;
}

// Fun��o para construir a �rvore de Huffman
No* construirArvoreHuffman(char caracteres[], int frequencias[], int tamanho) {
    No *esquerdo, *direito, *topo;
    Pilha* pilha = criarConstruirPilha(caracteres, frequencias, tamanho);

    while (!tamanhoUnico(pilha)) {
        esquerdo = extrairMinimo(pilha);
        direito = extrairMinimo(pilha);

        topo = novoNo('$', esquerdo->frequencia + direito->frequencia);
        topo->esquerdo = esquerdo;
        topo->direito = direito;

        inserirPilha(pilha, topo);
    }

    return extrairMinimo(pilha);
}

// Fun��o para imprimir os c�digos de Huffman
void imprimirCodigos(No* raiz, int arr[], int top) {
    if (raiz->esquerdo) {
        arr[top] = 0;
        imprimirCodigos(raiz->esquerdo, arr, top + 1);
    }

    if (raiz->direito) {
        arr[top] = 1;
        imprimirCodigos(raiz->direito, arr, top + 1);
    }

    if (!raiz->esquerdo && !raiz->direito) {
        printf("%c: ", raiz->caractere);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// Fun��o principal que executa a codifica��o de Huffman
void Huffman(char caracteres[], int frequencias[], int tamanho) {
    No* raiz = construirArvoreHuffman(caracteres, frequencias, tamanho);

    int arr[100], top = 0;
    imprimirCodigos(raiz, arr, top);
}

// Fun��o principal
int main() {
    char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequencias[] = {5, 9, 12, 13, 16, 45};
    int tamanho = sizeof(caracteres) / sizeof(caracteres[0]);

    printf("C�digos de Huffman para os caracteres dados:\n");
    Huffman(caracteres, frequencias, tamanho);

    return 0;
}
