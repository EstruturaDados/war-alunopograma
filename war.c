#include <stdio.h>   // Funções de entrada e saída (printf, scanf)
#include <stdlib.h>  // Funções utilitárias (calloc, free, rand)
#include <string.h>  // Manipulação de strings (strcpy, strcmp)
#include <time.h>    // Inicialização do gerador de números aleatórios (srand)


// Função para retornar o código ANSI da cor do exército
const char* corANSI(const char *cor) {
    if (strcmp(cor, "Vermelho") == 0) return "\033[31m";
    if (strcmp(cor, "Azul") == 0)     return "\033[34m";
    if (strcmp(cor, "Verde") == 0)    return "\033[32m";
    if (strcmp(cor, "Amarelo") == 0)  return "\033[33m";
    if (strcmp(cor, "cinza") == 0)    return "\033[90m"; // cinza
    return "\033[0m"; // padrão (sem cor)
}


// Constantes de configuração
#define MAX_NOME 50          // Tamanho máximo do nome do território
#define MAX_COR 20           // Tamanho máximo da cor do exército
#define MAX_TERRITORIOS 5    // Número fixo de territórios

// Estrutura que representa um território
typedef struct {
    char nome[MAX_NOME];       // Nome do território0
    char corExercito[MAX_COR]; // Cor do exército dominante
    int tropas;                // Número de tropas
} Territorio;

// Estrutura que representa uma missão
typedef struct {
    int tipo;        // 1 = destruir exército Verde, 2 = conquistar 3 territórios
    int progresso;   // progresso da missão
    int concluida;   // flag de missão concluída
} Missao;

// Inicializa os territórios automaticamente
void inicializarTerritorios(Territorio *mapa) {
    const char *nomes[] = {"Clã do Dragão Escarlate", "Clã da Serpente ", "Clã da Águia Dourada", "Clã do Lobo Cinzento", "Clã da Fênix Negra"};
    const char *cores[] = {"Vermelho", "Azul", "Verde", "Amarelo", "cinza"};

    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);              // Copia nome para o território
        strcpy(mapa[i].corExercito, cores[i]);       // Copia cor do exército
        mapa[i].tropas = 3 + rand() % 3;             // Define tropas iniciais entre 3 e 5
    }
}

// Exibe o estado atual do mapa
void mostrarMapa(const Territorio *mapa) {
    printf("\n=== Estado Atual do Mapa ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
       printf("%d - %s%s\033[0m | Exército: %s | Tropas: %d\n",
       i + 1,
       corANSI(mapa[i].corExercito),  // aplica a cor ANSI
       mapa[i].nome,                  // nome do clã colorido
       mapa[i].corExercito,           // texto normal
       mapa[i].tropas);

    }
}

// Atribui uma missão aleatória
void atribuirMissao(Missao *missao) {
    missao->tipo = rand() % 2 + 1;   // Sorteia missão (1 ou 2)
    missao->progresso = 0;           // Zera progresso
    missao->concluida = 0;           // Marca como não concluída

    printf("\n=== Missão atribuída ===\n");
    if (missao->tipo == 1) {
        printf("Missão: Destruir o exército Verde!\n");
    } else {
        printf("Missão: Conquistar 3 territórios!\n");
    }
}

// Verifica se a missão foi concluída
void verificarMissao(Missao *missao, const Territorio *mapa) {
    if (missao->tipo == 1) {
        int existeVerde = 0;
        for (int i = 0; i < MAX_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, "Verde") == 0) {
                existeVerde = 1; // ainda existe território verde
                break;
            }
        }
        if (!existeVerde) {
            missao->concluida = 1; // missão concluída
        }
    } else if (missao->tipo == 2) {
        if (missao->progresso >= 3) {
            missao->concluida = 1; // missão concluída
        }
    }

    if (missao->concluida) {
        printf("\n🎉 Missão concluída! Você venceu!\n");
    } else {
        printf("\nMissão ainda não concluída.\n");
    }
}

// Função que simula uma batalha
void batalha(Territorio *atacante, Territorio *defensor, Missao *missao) {
    int dadoAtaque = rand() % 6 + 1; // rola dado atacante
    int dadoDefesa = rand() % 6 + 1; // rola dado defensor

    printf("\n=== Confronto entre os Clãs ===\n");
    printf("🎲  %s lançou seu dado: %d\n", atacante->nome, dadoAtaque);
    printf("🛡️  %s lançou seu dado: %d\n", defensor->nome, dadoDefesa);

    // ⚖️ EMPATE — ninguém perde tropa
    if (dadoAtaque == dadoDefesa) {
        printf("⚖️  Os dados empataram! Nenhum clã levou vantagem nesta rodada.\n");
        return; // encerra a batalha sem mudanças
    }

    // Atacante vence o duelo de dados
    if (dadoAtaque > dadoDefesa) {
        defensor->tropas--; // defensor perde tropa
        printf("⚔️  %s sofreu uma baixa na defesa!\n", defensor->nome);

        // Se o defensor perdeu todas as tropas
        if (defensor->tropas <= 0) {
            printf("🔥  %s foi derrotado! Suas terras agora pertencem ao clã %s!\n",
                   defensor->nome, atacante->nome);

            defensor->tropas = 1; // novo território começa com 1 tropa
            strcpy(defensor->corExercito, atacante->corExercito);

            if (missao->tipo == 2) {
                missao->progresso++; // incrementa progresso da missão
            }
        }

    // Defensor vence o duelo de dados
    } else {
        atacante->tropas--; // atacante perde tropa
        printf("💥 O ataque de %s falhou! Uma tropa foi perdida no confronto!\n",
               atacante->nome);

        // Mensagem extra se o atacante estiver ficando fraco
        if (atacante->tropas == 1) {
            printf("⚠️  %s está enfraquecido e luta pela sobrevivência!\n",
                   atacante->nome);
        }
    }
}

// Menu principal
void menuPrincipal(Territorio *mapa, Missao *missao) {
    int opcao, atacante, defensor;

    do {
        printf("\n=== Menu Principal ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                mostrarMapa(mapa);
                printf("\nEscolha o território atacante (1 a %d): ", MAX_TERRITORIOS);
                scanf("%d", &atacante);
                printf("Escolha o território defensor (1 a %d): ", MAX_TERRITORIOS);
                scanf("%d", &defensor);

                if (atacante >= 1 && atacante <= MAX_TERRITORIOS &&
                    defensor >= 1 && defensor <= MAX_TERRITORIOS &&
                    atacante != defensor) {
                    batalha(&mapa[atacante - 1], &mapa[defensor - 1], missao);
                } else {
                    printf("Escolha inválida!\n");
                }
                break;

            case 2:
                verificarMissao(missao, mapa);
                break;

            case 0:
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0 && !missao->concluida);
}

// Função principal
int main() {
    srand(time(NULL)); // inicializa gerador de números aleatórios

    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof(Territorio)); // cria vetor de territórios
    Missao missao; // cria a missão

    inicializarTerritorios(mapa); // inicializa territórios
    atribuirMissao(&missao);      // sorteia missão

    menuPrincipal(mapa, &missao); // roda menu principal

    free(mapa); // libera memória
    printf("\n  Os clãs descansam em paz... Memória liberada! \n");
    return 0;   // encerra programa
}
