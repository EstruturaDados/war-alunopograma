#include <stdio.h>
#include <string.h>

#define MAX_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20

// Definição da struct Territorio
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int tropas;
} Territorio;

int main() {
    Territorio mapa[MAX_TERRITORIOS];
    int i;

    printf("=== Cadastro de Territórios ===\n");

    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nTerritório %d:\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remove \n

        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, MAX_COR, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // consome o \n deixado pelo scanf
    }

    printf("\n=== Estado Atual do Mapa ===\n");
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor do Exército: %s\n", mapa[i].corExercito);
        printf("  Tropas: %d\n", mapa[i].tropas);
    }

    return 0;
}
