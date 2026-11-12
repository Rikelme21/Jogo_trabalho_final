#include <raylib.h>
#include "jogo.h"

void processarCasa(Jogo *j){
    if(j->mapa[j->linhaJ][j->colunaJ] == 'C'){
        j->cristais = j->cristais + 1;
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'R'){
        j->energia = j->energia + 4;
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'A'){
        j->energia = j->energia - 4;
    }
    if(j->mapa[j->linhaJ][j->colunaJ] == 'E'){

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
    
    DrawText(TextFormat("Energias restantes: %d", j->energia),100, 250, 30, BLACK);
    DrawText(TextFormat("Cristais: %d", j->cristais),100, 290, 30, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos),100, 330, 30, BLACK);

    while(op3==0){
        BeginDrawing();

        DrawText("Jogar novamente?",100, 400, 30, BLACK);
        DrawText("Pressionar 'S' se sim, e 'N' se não",100, 430, 30, BLACK);

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
