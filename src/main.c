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
#include "jogo.h"

int main() {
    int resultado = TELA_INICIAL; // O jogo agora começa no Menu Inicial
    int reiniciar = 0;
    int qtdLetras = 0; // Quantas letras o nome tem no momento
    Jogo jogo;

    if (carregarMapa("mapa1.txt", &jogo) == 0) return 1; 
    
    jogo.energia = ENERGIA_INICIAL;
    jogo.cristais = 0;
    jogo.turnos = 0;
    jogo.nomeJogador[0] = '\0'; // Começa com o nome vazio
    strcpy(jogo.mensagem, "Explore a ilha!");

    InitWindow(800, 600, "Expedicao na Ilha dos Algoritmos");
    SetTargetFPS(30);

    while (!WindowShouldClose()) { 

        //Tela inicial(menu)
        if (resultado == TELA_INICIAL) {
            
            // Captura as letras do teclado
            int teclaChar = GetCharPressed();
            while (teclaChar > 0) {
                // Aceita apenas caracteres normais e limita a 20 letras
                if ((teclaChar >= 32) && (teclaChar <= 125) && (qtdLetras < 20)) {
                    jogo.nomeJogador[qtdLetras] = (char)teclaChar;
                    jogo.nomeJogador[qtdLetras + 1] = '\0';
                    qtdLetras++;
                }
                teclaChar = GetCharPressed(); // Pega a próxima letra se o utilizador digitou rápido
            }

            // Apagar a última letra com o Backspace
            if (IsKeyPressed(KEY_BACKSPACE)) {
                qtdLetras--;
                if (qtdLetras < 0) qtdLetras = 0;
                jogo.nomeJogador[qtdLetras] = '\0';
            }

            // Pressionar ENTER para começar o jogo (só permite se tiver digitado algum nome)
            if (IsKeyPressed(KEY_ENTER) && qtdLetras > 0) {
                resultado = EM_ANDAMENTO; 
            }

            // Desenho da Tela Inicial
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("EXPEDICAO NA ILHA DOS ALGORITMOS", 120, 150, 30, DARKBLUE);
            DrawText("Para comecar a aventura, digite o seu nome:", 120, 250, 20, DARKGRAY);
            
            // Caixa de texto
            DrawRectangle(120, 280, 400, 40, LIGHTGRAY);
            DrawText(jogo.nomeJogador, 130, 290, 20, BLACK);
            
            // Cursor a piscar
            if (qtdLetras < 20) {
                DrawText("_", 130 + MeasureText(jogo.nomeJogador, 20), 290, 20, DARKGRAY);
            }

            if (qtdLetras > 0) {
                DrawText("Pressione [ENTER] para Jogar", 120, 350, 20, GREEN);
            } else {
                DrawText("Aguardando nome...", 120, 350, 20, GRAY);
            }
            
            EndDrawing();
            continue; // Pula o resto do código e recomeça o while, para não rodar o jogo de fundo
        }
        
        // Jogo em andamento
        int comando = GetKeyPressed();

        if(reiniciar == 1) {
            carregarMapa("mapa1.txt", &jogo);
            jogo.energia = ENERGIA_INICIAL;
            jogo.cristais = 0;
            jogo.turnos = 0;
            strcpy(jogo.mensagem, "O jogo recomecou!");
            
            // Volta para a tela inicial se o jogador quiser recomeçar
            resultado = TELA_INICIAL; 
            jogo.nomeJogador[0] = '\0';
            qtdLetras = 0;
            reiniciar = 0;
            continue;
        }

        if(resultado == EM_ANDAMENTO) {
            if(comando == KEY_UP || comando == KEY_DOWN || comando == KEY_LEFT || comando == KEY_RIGHT) {
                if(movimentoValido(&jogo, comando) == 1) {
                    if(jogo.energia > 0) {
                        moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando); 
                        jogo.energia--;     
                        jogo.turnos++;      
                        processarCasa(&jogo); 
                        
                        if(jogo.energia <= 0 && !chegouNaSaida(&jogo)) {
                            resultado = DERROTA;
                        }
                    }
                }
            }
            if(comando == KEY_Q) resultado = DESISTENCIA;

            if(chegouNaSaida(&jogo)) {
                if(jogo.cristais >= CRISTAIS_VITORIA) resultado = VITORIA;
                else strcpy(jogo.mensagem, "Faltam cristais!");
            }
        }

        // Desenho do mapa
        if (resultado == EM_ANDAMENTO) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            desenharJogo(&jogo);       
            EndDrawing();
        } else {
            if(mostrarFimDeJogo(&jogo, resultado) == 1) reiniciar = 1;
            else break;
        }
    }

    CloseWindow(); 
    return 0;
}