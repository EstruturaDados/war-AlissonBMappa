// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
#include <stdio.h>
#include <string.h>

#define MAX 5
//Criar war conquista de territorio//
// Definição da struct
typedef struct {
    char nome[50];
    char corExercito[20];
    int tropas;
} Territorio;

int main() {
    Territorio territorios[MAX];

    // Cadastro dos 5 territórios
    for (int i = 0; i < MAX; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

        printf("Nome do territorio: ");
        scanf(" %[^\n]", territorios[i].nome);  // lê string com espaços

        printf("Cor do exercito: ");
        scanf(" %[^\n]", territorios[i].corExercito);

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos territórios cadastrados
    printf("\n===== Lista de Territorios Cadastrados =====\n");
    for (int i = 0; i < MAX; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].corExercito);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}
