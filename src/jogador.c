#include <raylib.h>
#include "jogo.h"

// o '*' indica que é um ponteiro, permitindo alterar o valor armazenado no endereço da memória
void moverJogador(int *linha, int *coluna, int comando) {
    if (comando == KEY_UP || comando == KEY_W) {
        *linha = *linha - 1;
    } else if (comando == KEY_DOWN || comando == KEY_S) {
        *linha = *linha + 1;
    } else if (comando == KEY_LEFT || comando == KEY_A) {
        *coluna = *coluna - 1;
    } else if (comando == KEY_RIGHT || comando == KEY_D) {
        *coluna = *coluna + 1;
    }
}