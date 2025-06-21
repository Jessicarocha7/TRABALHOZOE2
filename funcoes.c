#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    OCULTA,
    REVELADA,
    ENCONTRADA
} EstadoCarta;

typedef struct Carta {
    int id;
    char simbolo[3];
    EstadoCarta estado;
    struct Carta* prox;
} Carta;

typedef struct {
    char nome[50];
    int pontos;
} Jogador;

void limpar_buffer();
void ordenar_pontuacoes();
int contar_cartas(Carta* inicio);
void trocar_valores(Jogador* a, Jogador* b);

Carta* criar_baralho() {
    const char* simbolos[] = {"A", "B", "C", "D", "E", "F"};
    Carta* inicio = NULL;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            Carta* nova = malloc(sizeof(Carta));
            if (!nova) exit(1);
            nova->id = i;
            strcpy(nova->simbolo, simbolos[i]);
            nova->estado = OCULTA;
            nova->prox = inicio;
            inicio = nova;
        }
    }
    return inicio;
}

void embaralhar_cartas(Carta* baralho) {
    srand(time(NULL));
    int n = contar_cartas(baralho);
    Carta** vet = malloc(n * sizeof(Carta*));
    if (!vet) exit(1);

    Carta* atual = baralho;
    for (int i = 0; i < n; i++) {
        vet[i] = atual;
        atual = atual->prox;
    }

    for (int i = 0; i < n; i++) {
        int r = rand() % n;
        Carta temp = *vet[i];
        *vet[i] = *vet[r];
        *vet[r] = temp;
    }

    free(vet);
}

void exibir_tabuleiro(Carta* baralho) {
    Carta* atual = baralho;
    int pos = 1;
    printf("\nTabuleiro:\n");
    while (atual) {
        if (atual->estado == OCULTA)
            printf("[%2d] ", pos);
        else if (atual->estado == REVELADA || atual->estado == ENCONTRADA)
            printf(" %s  ", atual->simbolo);

        if (pos % 4 == 0) printf("\n");
        atual = atual->prox;
        pos++;
    }
}

Carta* encontrar_carta_por_posicao(Carta* baralho, int pos) {
    if (!baralho || pos < 1 || pos > 12) return NULL;
    int i = 1;
    Carta* atual = baralho;
    while (atual) {
        if (i == pos) return atual;
        atual = atual->prox;
        i++;
    }
    return NULL;
}

void liberar_baralho(Carta* baralho) {
    while (baralho) {
        Carta* temp = baralho;
        baralho = baralho->prox;
        free(temp);
    }
}

void salvar_pontuacao(Jogador* j1, Jogador* j2) {
    FILE* f = fopen("placar.txt", "a");
    if (!f) {
        printf("Erro ao gravar placar.\n");
        return;
    }
    fprintf(f, "%s;%d\n", j1->nome, j1->pontos);
    fprintf(f, "%s;%d\n", j2->nome, j2->pontos);
    fclose(f);
}

void exibir_placar() {
    printf("\n=== PLACAR ===\n");
    ordenar_pontuacoes();
}

void jogar() {
    Jogador j1 = {"", 0}, j2 = {"", 0};
    Carta* baralho = NULL;
    int jogada = 0, total_encontradas = 0;
    int pos1, pos2;

    printf("Nome do Jogador 1: ");
    scanf(" %30[^\n]", j1.nome);
    limpar_buffer();
    printf("Nome do Jogador 2: ");
    scanf(" %30[^\n]", j2.nome);
    limpar_buffer();

    baralho = criar_baralho();
    embaralhar_cartas(baralho);
    printf("Total de cartas criadas: %d\n", contar_cartas(baralho));


    while (total_encontradas < 6) {
        Jogador* atual = (jogada % 2 == 0) ? &j1 : &j2;
        printf("\nVez de %s\n", atual->nome);
        exibir_tabuleiro(baralho);

        printf("Escolha a primeira carta (1 a 12): ");
        scanf("%d", &pos1);
        printf("Escolha a segunda carta (1 a 12): ");
        scanf("%d", &pos2);

        Carta* c1 = encontrar_carta_por_posicao(baralho, pos1);
        Carta* c2 = encontrar_carta_por_posicao(baralho, pos2);

        if (!c1 || !c2 || c1 == c2 || c1->estado != OCULTA || c2->estado != OCULTA) {
            printf("Jogada invalida.\n");
            continue;
        }

        c1->estado = REVELADA;
        c2->estado = REVELADA;
        exibir_tabuleiro(baralho);

        if (c1->id == c2->id) {
            printf("Par encontrado!\n");
            c1->estado = ENCONTRADA;
            c2->estado = ENCONTRADA;
            atual->pontos++;
            total_encontradas++;
        } else {
            printf("Nao e um par.\n");
            c1->estado = OCULTA;
            c2->estado = OCULTA;
            jogada++;
        }
    }

    printf("\nFim de jogo!\nPontuacao:\n%s: %d pontos\n%s: %d pontos\n",
           j1.nome, j1.pontos, j2.nome, j2.pontos);

    salvar_pontuacao(&j1, &j2);
    liberar_baralho(baralho);
}
