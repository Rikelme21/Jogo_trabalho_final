/* 
-> para rodar o jogo no terminal do linux:
gcc -std=c11 -Wall -Wextra main.c jogador.c mapa.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./jogo
*/ 

#include <raylib.h>
#include "jogo.h"
#include <stdio.h>

int main(void) {
    int op1, op2;
    Jogo jogo; // cria variavel jogo do tipo Jogo ( como se fosse int jogo, mas ao invez de int é Jogo)

    jogo.linhaJ = 2; // inicia as variaveis
    jogo.colunaJ = 2;

    //criação do mapa
    jogo.linhas = 8;
    jogo.colunas = 10;
    for(op1 = 0; op1 < 8; op1++){
        for(op2 = 0; op2 < 10; op2++){
            if(op1 == 0 || op1 == 7 || op2 == 0 || op2 == 9){
                jogo.mapa[op1][op2] = '#';
            }else{
                jogo.mapa[op1][op2] = '.';
            }
            
        }
    }
    jogo.mapa[2][3] = '#';

    InitWindow(800, 600, "Teste Raylib"); // inicia a janela

    while (!WindowShouldClose()) { 
        int comando = GetKeyPressed();

        for(op1 = 0; op1 < 10; op1++){
            for(op2 = 0; op2 < 8; op2++){
                if(jogo.mapa[op2][op1]=='.'){
                    DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, GRAY);
                }
                if(jogo.mapa[op2][op1]=='#'){
                    DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, BLACK);
                }
            }
        }
        
        if(comando == KEY_UP || comando == KEY_DOWN || comando == KEY_LEFT || comando == KEY_RIGHT ){
            if(movimentoValido(&jogo, comando) == 1){
                moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando);
            }
            
            printf("Linha: %d | Coluna: %d\n", jogo.linhaJ, jogo.colunaJ); // teste pra ver se ta funcioandno
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle((jogo.colunaJ * 41)+40, (jogo.linhaJ * 41)+40, 40, 40, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
