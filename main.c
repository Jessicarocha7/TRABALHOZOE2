#include <stdio.h>

// Protótipos das funções de outros arquivos
void jogar();
void exibir_placar();

int main() {
    int opcao;

    do {
        printf("\n=== JOGO DA MEMÓRIA ===\n");
        printf("1. Jogar\n");
        printf("2. Ver Pontuacao\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2:
                exibir_placar();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);

    return 0;
}
