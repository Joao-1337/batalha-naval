# Batalha Naval em C

## Como jogar
Jogo de Batalha Naval simples feito em C, usando vetores e matrizes para representar o tabuleiro. Inclui ataques simples e ataques de área (bomba 3x3).

### Como compilar e executar
1. Compile o código:
   ```bash
   gcc -o batalha_naval batalha_naval.c
   ```
2. Execute o programa:
   ```bash
   ./batalha_naval
   ```

### Regras básicas
- Cada jogador tem 5 navios (ocupam 1 célula cada).
- O tabuleiro é 10x10.
- Ataque simples: afeta 1 célula.
- Bomba 3x3: afeta uma área centrada na posição escolhida.
- Ganha quem destruir todos os navios do inimigo.

### Conceitos aplicados
- Vetores e matrizes (tabuleiros 2D)
- Loops aninhados
- Condicionais dentro de loops
- Funções e modularização em C
