/* batalha_naval.c
   Jogo simplificado de Batalha Naval em C
   - Tabuleiro 10x10 (matriz)
   - Vetores/matrizes para armazenar tabuleiros
   - Loops aninhados e condicionais para exibir e checar área de efeito
   - Habilidade especial (bomba) que atinge uma área 3x3

   Compilar: gcc -o batalha_naval batalha_naval.c
   Executar: ./batalha_naval
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define N 10
#define SHIPS 5

void init_board(char board[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = '.';
}

void print_board_hidden(char board[N][N]) {
    printf("  ");
    for (int c = 0; c < N; c++) printf(" %d", c);
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%d ", i);
        for (int j = 0; j < N; j++) {
            char ch = board[i][j];
            if (ch == 'X' || ch == 'O') printf(" %c", ch);
            else printf(" %c", '.');
        }
        printf("\n");
    }
}

void print_board_full(char board[N][N]) {
    printf("  ");
    for (int c = 0; c < N; c++) printf(" %d", c);
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%d ", i);
        for (int j = 0; j < N; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

int in_bounds(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void place_ships_random(char board[N][N]) {
    int placed = 0;
    while (placed < SHIPS) {
        int r = rand() % N;
        int c = rand() % N;
        if (board[r][c] == '.') {
            board[r][c] = 'S';
            placed++;
        }
    }
}

int apply_single_shot(char target[N][N], char view[N][N], int r, int c) {
    if (!in_bounds(r, c)) return 0;
    if (view[r][c] == 'X' || view[r][c] == 'O') return -1;

    if (target[r][c] == 'S') {
        view[r][c] = 'X';
        return 1;
    } else {
        view[r][c] = 'O';
        return 0;
    }
}

int apply_area_bomb(char target[N][N], char view[N][N], int r, int c) {
    int hits = 0;
    for (int i = r - 1; i <= r + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (!in_bounds(i, j)) continue;
            if (view[i][j] == 'X' || view[i][j] == 'O') continue;
            if (target[i][j] == 'S') {
                view[i][j] = 'X';
                hits++;
            } else {
                view[i][j] = 'O';
            }
        }
    }
    return hits;
}

int count_ships_remaining(char target[N][N], char view[N][N]) {
    int rem = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (target[i][j] == 'S' && view[i][j] != 'X') rem++;
    return rem;
}

void player_turn(char enemy[N][N], char player_view[N][N]) {
    int r, c;
    char choice;
    printf("Sua vez!\n");
    print_board_hidden(player_view);
    printf("Escolha o tipo de ataque: (s)imples ou (b)omba 3x3? ");
    do {
        choice = getchar();
    } while (choice == '\n' || choice == '\r');
    while (getchar() != '\n');
    choice = tolower(choice);

    printf("Digite a linha (0-%d) e a coluna (0-%d), separados por espaço: ", N-1, N-1);
    if (scanf("%d %d", &r, &c) != 2) {
        printf("Entrada inválida. Pulando turno.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (choice == 'b') {
        int hits = apply_area_bomb(enemy, player_view, r, c);
        printf("Bomba aplicada! Navios atingidos nesta área: %d\n", hits);
    } else {
        int res = apply_single_shot(enemy, player_view, r, c);
        if (res == -1) printf("Você já atirou nessa posição.\n");
        else if (res == 1) printf("Acertou um navio!\n");
        else printf("Água...\n");
    }
}

void cpu_turn(char enemy[N][N], char cpu_view[N][N]) {
    int r, c;
    int use_bomb = (rand() % 5 == 0);
    do {
        r = rand() % N;
        c = rand() % N;
    } while (cpu_view[r][c] == 'X' || cpu_view[r][c] == 'O');

    if (use_bomb) {
        int hits = apply_area_bomb(enemy, cpu_view, r, c);
        printf("CPU usou bomba em (%d,%d) — navios atingidos: %d\n", r, c, hits);
    } else {
        int res = apply_single_shot(enemy, cpu_view, r, c);
        if (res == 1) printf("CPU acertou sua embarcação em (%d,%d)!\n", r, c);
        else if (res == 0) printf("CPU errou em (%d,%d).\n", r, c);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    char player_board[N][N];
    char cpu_board[N][N];
    char player_view[N][N];
    char cpu_view[N][N];

    init_board(player_board);
    init_board(cpu_board);
    init_board(player_view);
    init_board(cpu_view);

    place_ships_random(player_board);
    place_ships_random(cpu_board);

    printf("Bem-vindo à Batalha Naval simplificada!\n");
    printf("Você e a CPU têm %d navios cada. Primeiro a zerar os navios do oponente vence.\n", SHIPS);

    int turn = 0;
    while (1) {
        if (turn == 0) {
            printf("\nSeu tabuleiro (S = seus navios):\n");
            print_board_full(player_board);
            player_turn(cpu_board, player_view);
            int cpu_remaining = count_ships_remaining(cpu_board, player_view);
            if (cpu_remaining == 0) {
                printf("Parabéns! Você venceu — todos os navios inimigos afundaram!\n");
                break;
            }
            turn = 1;
        } else {
            printf("\nTurno da CPU...\n");
            cpu_turn(player_board, cpu_view);
            int player_remaining = count_ships_remaining(player_board, cpu_view);
            if (player_remaining == 0) {
                printf("A CPU venceu. Seus navios foram todos destruídos.\n");
                printf("Seu tabuleiro final:\n");
                print_board_full(player_board);
                break;
            }
            turn = 0;
        }
    }

    printf("Jogo encerrado. Obrigado por jogar!\n");
    return 0;
}
