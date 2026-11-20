#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"

// Verifica em qual letra o jogador pisou e aplica a regra
void processarCasa(Jogo *j, int comando, int *resultado) {
    char casaAtual = j->mapa[j->linhaJ][j->colunaJ];

    if (casaAtual == 'C') {
        j->cristais++;
        strcpy(j->mensagem, "Cristal coletado!");
    }
    else if (casaAtual == 'R') {
        j->energia += 4;
        strcpy(j->mensagem, "Recuperou 4 de energia!");
    }
    else if (casaAtual == 'A') {
        j->energia -= 4;
        if (j->energia < 0) j->energia = 0;
        strcpy(j->mensagem, "Armadilha! Perdeu 4 de energia.");
    }
    else if (casaAtual == 'E') {
        int sorteio = rand() % 4;
        j->mapa[j->linhaJ][j->colunaJ] = '.'; // Apaga o 'E' antes de um eventual teleporte

        switch (sorteio) {
            case 0:
                j->cristais++;
                strcpy(j->mensagem, "Evento: Ganhou 1 cristal!");
                break;
            case 1:
                j->energia += 4;
                strcpy(j->mensagem, "Evento: Ganhou 4 de energia!");
                break;
            case 2:
                j->energia -= 4;
                if (j->energia < 0) j->energia = 0;
                strcpy(j->mensagem, "Evento: Perdeu 4 de energia!");
                break;
            case 3:
                j->linhaJ = j->linhaInicial;
                j->colunaJ = j->colunaInicial;
                strcpy(j->mensagem, "Evento: Voltou ao inicio!");
                break;
        }
    }
    else if (casaAtual == 'S') {
        strcpy(j->mensagem, "Chegou na saida!");
    }
    else if (casaAtual == 'B'){
        if (comando == KEY_UP || comando == KEY_W){
            j->mapa[j->linhaJ - 1][j->colunaJ] = 'B';
        }
        if (comando == KEY_DOWN || comando == KEY_S){
            j->mapa[j->linhaJ + 1][j->colunaJ] = 'B';
        }
        if (comando == KEY_LEFT || comando == KEY_A){
            j->mapa[j->linhaJ][j->colunaJ - 1] = 'B';
        }
        if (comando == KEY_RIGHT || comando == KEY_D){
            j->mapa[j->linhaJ][j->colunaJ + 1] = 'B';
        }
    }
    else if (casaAtual == 'V') {
        *resultado = DERROTA;
        strcpy(j->mensagem, "Caiu no buraco!");
    }
    else {
        strcpy(j->mensagem, "Caminho livre...");
    }
    if (casaAtual != 'S' && casaAtual != 'E' && casaAtual != 'b' && casaAtual != 'V') {
        j->mapa[j->linhaJ][j->colunaJ] = '.';
    }
    
}

// Retorna 1 (Verdadeiro) se pisou na saída, e 0 (Falso) caso contrário
int chegouNaSaida(Jogo *j) {
    if(j->mapa[j->linhaJ][j->colunaJ] == 'S') {
        return 1;
    }
    return 0;
}

// Monta a tela de fim de jogo
int mostrarFimDeJogo(Jogo *j, int resultado) {
    int escolheu = 0;
    int opcao = 0;

    // Sub-laço: mantém a tela ativa até o jogador pressionar R ou Q
    while (escolheu == 0) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (resultado == VITORIA) {
            DrawText("VITORIA!", 100, 100, 80, GREEN);
            DrawText("Voce escapou da ilha!", 100, 190, 30, DARKGREEN);
        }
        else if (resultado == DERROTA) {
            DrawText("DERROTA", 100, 100, 80, RED);
            if(j->energia < 1){
                DrawText("Sua energia acabou...", 100, 190, 30, MAROON);
            }
            if(j->mapa[j->linhaJ][j->colunaJ] == 'V'){
                DrawText("Voce caiu no buraco...", 100, 190, 30, MAROON);
            }
        }
        else if (resultado == DESISTENCIA) {
            DrawText("DESISTENCIA", 100, 100, 80, ORANGE);
            DrawText("Voce abandonou a expedicao.", 100, 190, 30, DARKGRAY);
        }
        
        // Placar Final
        DrawText(TextFormat("Explorador: %s", j->nomeJogador), 100, 250, 25, DARKBLUE);
        DrawText(TextFormat("Energia restante: %d", j->energia), 100, 290, 25, BLACK);
        DrawText(TextFormat("Cristais coletados: %d", j->cristais), 100, 325, 25, BLACK);
        DrawText(TextFormat("Turnos gastos: %d", j->turnos), 100, 360, 25, BLACK);

        DrawText("O que deseja fazer?", 100, 430, 30, BLACK);
        DrawText("Pressione [R] para Reiniciar ou [Q] para Sair", 100, 470, 20, GRAY);

        EndDrawing();

        // Aguarda a resposta do teclado
        if (IsKeyPressed(KEY_R)) {
            opcao = 1; // 1 = reiniciar
            escolheu = 1;
            if(resultado == VITORIA){
                j->nivel = 1;
            }
        } else if (IsKeyPressed(KEY_Q)) {
            opcao = 0; // 0 = sair do jogo
            escolheu = 1;
        }

        // Proteção caso o jogador clique no "X" da janela
        if (WindowShouldClose()) {
            return 0;
        }
    }
    
    return opcao;
}