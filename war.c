// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct Território
typedef struct {
    char nome[50];
    char corExercito[20];
    int tropas;
} Territorio;

/* Funções auxiliares */
int compara_desc(const void *a, const void *b) {
    return ((int)b) - ((int)a);
}

void rolar_dados(int dados[], int n) {
    for (int i = 0; i < n; i++) {
        dados[i] = (rand() % 6) + 1; // 1..6
    }
    qsort(dados, n, sizeof(int), compara_desc);
}

void listar_territorios(Territorio *territorios, int n) {
    printf("\n===== Lista de Territorios =====\n");
    printf("%-3s | %-20s | %-15s | %-7s\n", "ID", "Nome", "Cor do Exército", "Tropas");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | %-15s | %-7d\n", 
               i + 1, territorios[i].nome, territorios[i].corExercito, territorios[i].tropas);
    }
}

/* Função ataque */
void atacar(Territorio *territorios, int idAtacante, int idDefensor, int tropasAtaque, int n) {
    Territorio *A = &territorios[idAtacante];
    Territorio *D = &territorios[idDefensor];

    if (idAtacante == idDefensor) {
        printf("Erro: atacante e defensor devem ser diferentes.\n");
        return;
    }
    if (A->tropas <= 1) {
        printf("Erro: %s não tem tropas suficientes para atacar.\n", A->nome);
        return;
    }
    if (tropasAtaque < 1 || tropasAtaque > 3 || tropasAtaque > A->tropas - 1) {
        printf("Erro: número de tropas inválido.\n");
        return;
    }

    int dadosA[3], dadosD[2];
    int nA = tropasAtaque;
    int nD = (D->tropas >= 2) ? 2 : 1;

    printf("\n--- Batalha: %s (Atacante) vs %s (Defensor) ---\n", A->nome, D->nome);

    rolar_dados(dadosA, nA);
    rolar_dados(dadosD, nD);

    printf("Dados atacante: ");
    for (int i = 0; i < nA; i++) printf("%d ", dadosA[i]);
    printf("\nDados defensor: ");
    for (int i = 0; i < nD; i++) printf("%d ", dadosD[i]);
    printf("\n");

    int comparacoes = (nA < nD) ? nA : nD;
    int perdasA = 0, perdasD = 0;

    for (int i = 0; i < comparacoes; i++) {
        if (dadosA[i] > dadosD[i]) perdasD++;
        else perdasA++;
    }

    // Atualiza tropas
    A->tropas -= perdasA;
    D->tropas -= perdasD;

    printf("Resultado: Atacante perdeu %d, Defensor perdeu %d\n", perdasA, perdasD);

    // Se o defensor perdeu tudo → conquista
    if (D->tropas <= 0) {
        int tropasMover = (tropasAtaque - perdasA > 0) ? tropasAtaque - perdasA : 1;
        if (tropasMover > A->tropas) tropasMover = A->tropas;

        A->tropas -= tropasMover;
        D->tropas = tropasMover;
        strcpy(D->corExercito, A->corExercito);

        printf("\n** Conquista! %s tomou %s com %d tropas. **\n", A->nome, D->nome, tropasMover);
    }

    // Atualização pós-ataque
    listar_territorios(territorios, n);
}

int main() {
    srand(time(NULL));

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    Territorio *territorios = malloc(n * sizeof(Territorio));
    if (!territorios) {
        printf("Erro de memoria.\n");
        return 1;
    }

    // Cadastro
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", territorios[i].nome);

        printf("Cor do Exército: ");
        scanf(" %[^\n]", territorios[i].corExercito);

        printf("Tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1 - Listar territorios\n");
        printf("2 - Atacar\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            listar_territorios(territorios, n);
        } else if (opcao == 2) {
            listar_territorios(territorios, n);
            int a, d, q;
            printf("ID Atacante: ");
            scanf("%d", &a);
            printf("ID Defensor: ");
            scanf("%d", &d);

            if (a < 1 || a > n || d < 1 || d > n) {
                printf("IDs inválidos.\n");
                continue;
            }
            printf("Quantas tropas usar no ataque? (1-3, máx. tropas-1): ");
            scanf("%d", &q);

            atacar(territorios, a - 1, d - 1, q, n);
        }
    } while (opcao != 3);

    free(territorios);
    return 0;
}