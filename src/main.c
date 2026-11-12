/* 
-> para rodar o jogo no terminal do linux:
gcc -std=c11 -Wall -Wextra main.c jogador.c mapa.c eventos.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./jogo
*/ 

#include <raylib.h>
#include "jogo.h"

int main() {
    int resultado = EM_ANDAMENTO, op1, op2, reiniciar = 0;
    Jogo jogo; // cria variavel jogo do tipo Jogo ( como se fosse int jogo, mas ao invez de int é Jogo)

    jogo.linhaJ = 2; // inicia as variaveis
    jogo.colunaJ = 2;

    jogo.energia = ENERGIA_INICIAL;
    jogo.cristais = 0;

    jogo.linhas = LINHAS;
    jogo.colunas = COLUNAS;

    for(op1 = 0; op1 < LINHAS; op1++){
        for(op2 = 0; op2 < COLUNAS; op2++){
            if(op1 == 0 || op1 == (LINHAS - 1) || op2 == 0 || op2 == (COLUNAS - 1)){
                jogo.mapa[op1][op2] = '#';
            }else{
                jogo.mapa[op1][op2] = '.';
            }
            
        }
    }

    jogo.mapa[2][3] = '#';
    jogo.mapa[5][3] = 'R';
    jogo.mapa[5][7] = 'A';
    jogo.mapa[6][6] = 'C';
    jogo.mapa[2][4] = 'C';
    jogo.mapa[2][5] = 'C';
    jogo.mapa[3][8] = 'S';

    InitWindow(800, 600, "Meu jogo"); // inicia a janela

    while (!WindowShouldClose()) { 
        int comando = GetKeyPressed();

        if(reiniciar == 1){

            //configuração do mapa
            resultado = EM_ANDAMENTO;

            jogo.linhaJ = 2; 
            jogo.colunaJ = 2;

            jogo.energia = ENERGIA_INICIAL;
            jogo.cristais = 0;

            jogo.linhas = LINHAS;
            jogo.colunas = COLUNAS;

            for(op1 = 0; op1 < LINHAS; op1++){
                for(op2 = 0; op2 < COLUNAS; op2++){
                    if(op1 == 0 || op1 == (LINHAS - 1) || op2 == 0 || op2 == (COLUNAS - 1)){
                        jogo.mapa[op1][op2] = '#';
                    }else{
                        jogo.mapa[op1][op2] = '.';
                    }
                    
                }
            }

            jogo.mapa[2][3] = '#';
            jogo.mapa[5][3] = 'R';
            jogo.mapa[5][7] = 'A';
            jogo.mapa[6][6] = 'C';
            jogo.mapa[2][4] = 'C';
            jogo.mapa[2][5] = 'C';
            jogo.mapa[3][8] = 'S';

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
                    processarCasa(&jogo);
                }else{
                    resultado = DERROTA;
                }
            }
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
            if(jogo.cristais >= CRISTAIS_VITORIA){
                resultado = VITORIA;
            }
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
