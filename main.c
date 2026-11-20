/* 
-> para rodar o jogo no terminal do linux:
gcc -std=c11 -Wall -Wextra main.c jogador.c mapa.c eventos.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./jogo
*/ 
/* 
-> para rodar o jogo no terminal do vscode:
gcc -std=c11 -Wall -Wextra *.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./a.out
*/ 

#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"

int main() {
    int resultado = TELA_INICIAL; 
    int reiniciar = 0;
    int qtdLetras = 0; 
    Jogo jogo;

    // Inicia no Nível 1 (carregarMapa já define jogo.energia e jogo.minimoCristais)
    jogo.nivel = 1; 
    if (carregarMapa("fases.txt", &jogo, jogo.nivel) == 0) return 1; 
    
    jogo.cristais = 0;
    jogo.turnos = 0;
    jogo.nomeJogador[0] = '\0'; 
    strcpy(jogo.mensagem, "Explore a ilha!");
    InitWindow(1280, 720, "Expedicao na Ilha dos Algoritmos");
    SetTargetFPS(30);
    srand(time(NULL));

    while (!WindowShouldClose()) { 
        
        // TELA INICIAL (MENU)
        if (resultado == TELA_INICIAL) {
            int teclaChar = GetCharPressed();
            while (teclaChar > 0) {
                if ((teclaChar >= 32) && (teclaChar <= 125) && (qtdLetras < 20)) {
                    jogo.nomeJogador[qtdLetras] = (char)teclaChar;
                    jogo.nomeJogador[qtdLetras + 1] = '\0';
                    qtdLetras++;
                }
                teclaChar = GetCharPressed(); 
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                qtdLetras--;
                if (qtdLetras < 0) qtdLetras = 0;
                jogo.nomeJogador[qtdLetras] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && qtdLetras > 0) {
                resultado = EM_ANDAMENTO; 
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("EXPEDICAO NA ILHA DOS ALGORITMOS", 350, 150, 30, DARKBLUE);
            DrawText("Para comecar a aventura, digite o seu nome:", 350, 250, 20, DARKGRAY);
            
            DrawRectangle(350, 280, 400, 40, LIGHTGRAY);
            DrawText(jogo.nomeJogador, 360, 290, 20, BLACK);
            
            if (qtdLetras < 20) {
                DrawText("_", 360 + MeasureText(jogo.nomeJogador, 20), 290, 20, DARKGRAY);
            }

            if (qtdLetras > 0) DrawText("Pressione [ENTER] para Jogar", 350, 350, 20, GREEN);
            else DrawText("Aguardando nome...", 350, 350, 20, GRAY);
            
            EndDrawing();
            
            continue; 
        }
        
        // ESTADO 2: JOGO EM ANDAMENTO
        int comando = GetKeyPressed();

        if (reiniciar == 1) {
            carregarMapa("fases.txt", &jogo, jogo.nivel);
            jogo.cristais = 0;
            jogo.turnos = 0;
            strcpy(jogo.mensagem, "O jogo recomecou!");
            
            resultado = EM_ANDAMENTO; 
            jogo.nomeJogador[0] = '\0';
            qtdLetras = 0;
            reiniciar = 0;
            continue;
        }

        if (resultado == EM_ANDAMENTO) {
            if (comando == KEY_UP || comando == KEY_W ||
                comando == KEY_DOWN || comando == KEY_S ||
                comando == KEY_LEFT || comando == KEY_A ||
                comando == KEY_RIGHT || comando == KEY_D) {
                if (movimentoValido(&jogo, comando) == 1) {
                    if (jogo.energia > 0) {
                        moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando); 
                        jogo.energia--;     
                        jogo.turnos++;      
                        processarCasa(&jogo, comando); 
                    }
                }
            }
            if (comando == KEY_Q) resultado = DESISTENCIA;

            // Verifica VITÓRIA / avanço de fase antes da derrota
            if (chegouNaSaida(&jogo)) {
                if (jogo.cristais >= jogo.minimoCristais) {
                    jogo.nivel++; 
                    if (carregarMapa("fases.txt", &jogo, jogo.nivel) == 1) {
                        jogo.cristais = 0;
                        strcpy(jogo.mensagem, "Avancou de Fase!");
                    } else {
                        resultado = VITORIA; 
                    }
                } else {
                    strcpy(jogo.mensagem, "Faltam cristais!");
                }
            }

            // Verifica DERROTA por falta de energia
            if (resultado == EM_ANDAMENTO && jogo.energia <= 0) {
                resultado = DERROTA;
            }
        }

        if (resultado == EM_ANDAMENTO) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            desenharJogo(&jogo);       
            EndDrawing();
        } else {
            if (mostrarFimDeJogo(&jogo, resultado) == 1) reiniciar = 1;
            else break;
        }
    }

    CloseWindow(); 
    return 0;
}