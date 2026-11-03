#include <raylib.h>
#include "jogo.h"
#include <stdio.h>

int main(void) {
    Jogo jogo; // cria variavel jogo do tipo jogo
    jogo.linhaJ = 2; // inicia as variaveis
    jogo.colunaJ = 3;

    InitWindow(800, 600, "Teste Raylib"); // inicia a janela

    while (!WindowShouldClose()) { 
        int comando = GetKeyPressed();
        if(comando == KEY_UP || comando == KEY_DOWN || comando == KEY_LEFT || comando == KEY_RIGHT ){
            moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando);
            printf("Linha: %d | Coluna: %d\n", jogo.linhaJ, jogo.colunaJ); // teste pra ver se ta funcioandno
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
