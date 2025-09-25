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

// ---------- Structs ----------
typedef struct {
    char nome[50];
    char corExercito[20];
    int tropas;
    int continente;
} Territorio;

typedef struct {
    char descricao[150];
    int tipo; // 1=conquistar X territorios, 2=eliminar jogador, 3=conquistar com tropas min, 4=controlar continente
    int valor;
    char alvo[20];
} Missao;

typedef struct {
    char corExercito[20];
    Missao missao;
    int ia; // 0 = humano, 1 = IA
} Jogador;

// ---------- Funções Territórios ----------
void listar_territorios(Territorio *territorios, int n) {
    printf("\n===== Lista de Territorios =====\n");
    printf("%-3s | %-20s | %-15s | %-7s | %-10s\n", "ID", "Nome", "Cor do Exército", "Tropas", "Continente");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | %-15s | %-7d | %-10d\n",
               i + 1, territorios[i].nome, territorios[i].corExercito, territorios[i].tropas, territorios[i].continente);
    }
}

Territorio* cadastrar_territorios(int n) {
    Territorio *territorios = malloc(n * sizeof(Territorio));
    if (!territorios) exit(1);

    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", territorios[i].nome);
        printf("Cor do Exército: ");
        scanf(" %[^\n]", territorios[i].corExercito);
        printf("Tropas: ");
        scanf("%d", &territorios[i].tropas);
        printf("Continente (1-3): ");
        scanf("%d", &territorios[i].continente);
    }
    return territorios;
}

// ---------- Funções Batalha ----------
static int compara_desc(const void a, const void *b) { return ((int*)b) - ((int)a); }
static void rolar_dados(int dados[], int n) { for(int i=0;i<n;i++) dados[i]=(rand()%6)+1; qsort(dados,n,sizeof(int),compara_desc); }

void atacar(Territorio *territorios, int idAtacante, int idDefensor, int tropasAtaque, int n) {
    Territorio *A = &territorios[idAtacante];
    Territorio *D = &territorios[idDefensor];

    if (idAtacante==idDefensor || A->tropas<=1 || tropasAtaque<1 || tropasAtaque>3 || tropasAtaque>A->tropas-1) return;

    int dadosA[3], dadosD[2], nA=tropasAtaque, nD=(D->tropas>=2)?2:1;
    rolar_dados(dadosA,nA); rolar_dados(dadosD,nD);

    int comparacoes=(nA<nD)?nA:nD, perdasA=0, perdasD=0;
    for(int i=0;i<comparacoes;i++) if(dadosA[i]>dadosD[i]) perdasD++; else perdasA++;

    A->tropas-=perdasA; D->tropas-=perdasD;

    if(D->tropas<=0){
        int tropasMover=(tropasAtaque-perdasA>0)?tropasAtaque-perdasA:1;
        if(tropasMover>A->tropas) tropasMover=A->tropas;
        A->tropas-=tropasMover;
        D->tropas=tropasMover;
        strcpy(D->corExercito,A->corExercito);
    }
}

// ---------- Funções Missões ----------
Missao missoesDisponiveis[]={
    {"Conquistar 5 territorios",1,5,""},
    {"Conquistar 3 territorios com pelo menos 5 tropas cada",3,3,""},
    {"Eliminar jogador vermelho",2,0,"vermelho"},
    {"Controlar o continente 1",4,0,"1"}
};
int totalMissoes=sizeof(missoesDisponiveis)/sizeof(Missao);

void sortear_missao(Jogador *jogador){ int idx=rand()%totalMissoes; jogador->missao=missoesDisponiveis[idx]; }

int verificar_vitoria(Jogador *jogador,Territorio *territorios,int n,Jogador *jogadores,int numJogadores){
    Missao *m=&jogador->missao; int count=0;
    for(int i=0;i<n;i++){
        if(strcmp(territorios[i].corExercito,jogador->corExercito)==0){
            if(m->tipo==1) count++;
            if(m->tipo==3 && territorios[i].tropas>=5) count++;
            if(m->tipo==4 && territorios[i].continente==atoi(m->alvo)) count++;
        }
    }
    if(m->tipo==2){ int vivo=0; for(int j=0;j<numJogadores;j++) if(strcmp(jogadores[j].corExercito,m->alvo)==0) vivo=1; return !vivo; }
    return count>=m->valor;
}

// ---------- Funções IA ----------
void jogar_ia(Jogador *ia, Territorio *territorios, int n){
    int possiveis[50][2]; int tot=0;
    for(int i=0;i<n;i++) if(strcmp(territorios[i].corExercito,ia->corExercito)==0 && territorios[i].tropas>1){
        for(int j=0;j<n;j++) if(strcmp(territorios[j].corExercito,ia->corExercito)!=0) { possiveis[tot][0]=i; possiveis[tot][1]=j; tot++; }
    }
    if(tot>0){
        int idx=rand()%tot;
        int tropas=(territorios[possiveis[idx][0]].tropas>3)?3:territorios[possiveis[idx][0]].tropas-1;
        atacar(territorios,possiveis[idx][0],possiveis[idx][1],tropas,n);
        printf("\nIA %s atacou %s\n", ia->corExercito, territorios[possiveis[idx][1]].nome);
    }
}

// ---------- Main ----------
int main(){
    srand(time(NULL));

    int n; printf("Quantos territorios deseja cadastrar? "); scanf("%d",&n);
    Territorio *territorios=cadastrar_territorios(n);

    int numJogadores; printf("Quantos jogadores? "); scanf("%d",&numJogadores);
    Jogador *jogadores=malloc(numJogadores*sizeof(Jogador));

    for(int i=0;i<numJogadores;i++){
        printf("\n--- Jogador %d ---\n",i+1);
        printf("Cor do exercito: "); scanf(" %[^\n]", jogadores[i].corExercito);
        printf("É IA? (0=não,1=sim): "); scanf("%d",&jogadores[i].ia);
        sortear_missao(&jogadores[i]);
        printf("Missão: %s\n", jogadores[i].missao.descricao);
    }

    int jogadorAtual=0, opcao;
    do{
        Jogador *atual=&jogadores[jogadorAtual];
        printf("\n=== TURNO: %s ===\n", atual->corExercito);

        if(atual->ia){
            jogar_ia(atual,territorios,n);
            if(verificar_vitoria(atual,territorios,n,jogadores,numJogadores)){
                printf("\n🎉 IA %s venceu sua missão: %s 🎉\n", atual->corExercito, atual->missao.descricao);
                break;
            }
        } else {
            printf("1-Listar territorios\n2-Atacar\n3-Sair\nEscolha: ");
            scanf("%d",&opcao);
            if(opcao==1) listar_territorios(territorios,n);
            else if(opcao==2){
                int a,d,q; listar_territorios(territorios,n);
                printf("ID Atacante: "); scanf("%d",&a);
                printf("ID Defensor: "); scanf("%d",&d);
                printf("Quantas tropas usar? "); scanf("%d",&q);
                atacar(territorios,a-1,d-1,q,n);
                if(verificar_vitoria(atual,territorios,n,jogadores,numJogadores)){
                    printf("\n🎉 Jogador %s venceu sua missão: %s 🎉\n", atual->corExercito, atual->missao.descricao);
                    break;
                }
            } else if(opcao==3) break;
        }

        jogadorAtual=(jogadorAtual+1)%numJogadores;
    } while(1);

    free(territorios); free(jogadores);
    return 0;
}