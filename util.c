#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipos de dados duplicados localmente
typedef struct {
    char nome[50];
    int pontos;
} Jogador;

typedef struct Carta {
    int id;
    char simbolo[3];
    int estado;
    struct Carta* prox;
} Carta;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int contar_cartas(Carta* inicio) {
    int count = 0;
    while (inicio) {
        count++;
        inicio = inicio->prox;
    }
    return count;
}

void trocar_valores(Jogador* a, Jogador* b) {
    Jogador temp = *a;
    *a = *b;
    *b = temp;
}

void ordenar_pontuacoes() {
    FILE* f = fopen("placar.txt", "r");
    if (!f) {
        printf("Sem placar registrado ainda.\n");
        return;
    }

    Jogador lista[100];
    int count = 0;

    while (fscanf(f, " %49[^;];%d\n", lista[count].nome, &lista[count].pontos) == 2) {
        count++;
    }
    fclose(f);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (lista[j].pontos > lista[i].pontos)
                trocar_valores(&lista[i], &lista[j]);
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s - %d pontos\n", lista[i].nome, lista[i].pontos);
    }
}
