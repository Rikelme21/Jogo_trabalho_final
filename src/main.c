/* 
-> para rodar o jogo no terminal do linux:
gcc -std=c11 -Wall -Wextra main.c jogador.c mapa.c eventos.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./jogo
*/ 

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jogo.h"

int main() {
    int resultado = EM_ANDAMENTO, reiniciar = 0, validez = 1, comando;
    Jogo jogo; // cria variavel jogo do tipo Jogo ( como se fosse int jogo, mas ao invez de int é Jogo)

    //puxa o mapa txt e verifica se é possivel
    if(carregarMapa("mapa1.txt", &jogo) == 0){
        InitWindow(800, 600, "Meu jogo"); // inicia a janela
        while (!WindowShouldClose()) { 
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Erro no arquivo mapa",100, 500, 30, BLACK);
            EndDrawing();

        }
        CloseWindow();
        return 0;
    }

    //para eventos aleatorios
    srand((unsigned int)time(NULL));

    InitWindow(800, 600, "Meu jogo"); // inicia a janela

    
    while (!WindowShouldClose()) { 
        comando = GetKeyPressed();

        //reinicia a configuração inicial do jogo
        if(reiniciar == 1){
            resultado = EM_ANDAMENTO;
            if(carregarMapa("mapa1.txt", &jogo) == 0){
            }    
            reiniciar = 0;
        }

        //desenha o mapa
        if(resultado == EM_ANDAMENTO){
            desenharJogo(&jogo);
        }
        
        //movimenta jogador
        if(comando == KEY_UP || comando == KEY_DOWN || comando == KEY_LEFT || comando == KEY_RIGHT ){
            if(movimentoValido(&jogo, comando) == 1){
                if(jogo.energia > 0){
                    moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando);
                    jogo.energia = jogo.energia - 1;
                    if(validez && jogo.mapa[jogo.linhaJ][jogo.colunaJ] != 'E'){
                        jogo.mensagem[0] = '\0';
                    }
                    processarCasa(&jogo);
                    jogo.turnos++;
                    validez = 1;
                }else{
                    resultado = DERROTA;
                }
            }else{
                validez = 0;
            }
        }

        if(validez == 0){
            strcpy(jogo.mensagem, "Movimento invalido!");
        }

        //se o player apertar Q, o jogo é desistido
        if(comando == KEY_Q){
            resultado = DESISTENCIA;
            if(mostrarFimDeJogo(&jogo, resultado) == 1){
                reiniciar = 1;
            }else{
                break;
            }
        }

        //roda se a casa for a saida
        if(chegouNaSaida(&jogo)){
            if(jogo.cristais >= jogo.minimoCristais){
                resultado = VITORIA;
            }
        }

        //se a energia for igual ou menor que zero e ainda não esta no final, entã
        if(jogo.energia <= 0 && !chegouNaSaida(&jogo)){
            resultado = DERROTA;
        }

        //exibe a tela final
        if(resultado != EM_ANDAMENTO){
            if(mostrarFimDeJogo(&jogo, resultado) == 1){
                reiniciar = 1;
                
            }else{
                break;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
