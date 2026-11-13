#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"

void processarCasa(Jogo *j){
    int sorteio;

    if(j->mapa[j->linhaJ][j->colunaJ] == 'C'){
        j->cristais = j->cristais + 1;
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'R'){
        j->energia = j->energia + 4;
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'A'){
        j->energia = j->energia - 4;
        if(j->energia < 0){
            j->energia = 0;
        }
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'E'){
        sorteio = rand() %4;
        j->mapa[j->linhaJ][j->colunaJ] = '.';

        switch (sorteio){
            case 0:
                j->cristais = j->cristais + 1;
                strcpy(j->mensagem, "Evento misterioso: Voce ganhou um cristal!");
            break;
            case 1:
                j->energia = j->energia + 4;
                strcpy(j->mensagem, "Evento misterioso: Voce ganhou 4 energias!");

            break;
            case 2:
                j->energia = j->energia - 4;
                if(j->energia < 0){
                    j->energia = 0;
                }
                strcpy(j->mensagem, "Evento misterioso: Voce perdeu 4 energias!");


            break;
            case 3:
                j->linhaJ = j->linhaInicial;
                j->colunaJ = j->colunaInicial;
                strcpy(j->mensagem, "Evento misterioso: Você voltou para o posição inicial!");
            break;
        }
    }
    if(j->mapa[j->linhaJ][j->colunaJ] != 'S'){
        j->mapa[j->linhaJ][j->colunaJ] = '.';
    }
}


int chegouNaSaida(Jogo *j){
    if(j->mapa[j->linhaJ][j->colunaJ] == 'S'){
        return 1;
    }else{
        return 0;
    }
}


int mostrarFimDeJogo(Jogo *j, int resultado){
    int op3 = 0;

    //desenha tela final
    DrawRectangle(1, 1, 800, 600, WHITE);

    if(resultado == VITORIA){
        DrawText("VITORIA!",100, 100, 100, GREEN);
    }
    if(resultado == DERROTA){
        DrawText("DERROTA",100, 100, 100, RED);
    }
    if(resultado == DESISTENCIA){
        DrawText("VOCÊ",100, 100, 100, ORANGE);
        DrawText("DESISTIU",100, 210, 100, ORANGE);
    }
    
    DrawText(TextFormat("Energias restantes: %d", j->energia),100, 330, 30, BLACK);
    DrawText(TextFormat("Cristais: %d", j->cristais),100, 370, 30, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos),100, 410, 30, BLACK);

    //opção de jogar novamente
    while(op3==0){
        BeginDrawing();

        DrawText("Jogar novamente?",100, 470, 30, BLACK);
        DrawText("Pressionar 'S' se sim, e 'N' se não",100, 500, 30, BLACK);

        if(IsKeyPressed(KEY_S)){
            return 1;
        }else{
            if(IsKeyPressed(KEY_N)){
                return 0;
            }
        }

        EndDrawing();
        if (WindowShouldClose()) {
            return 0;
        }

    }
    return 0;
}
