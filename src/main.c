/* 
-> para rodar o jogo no terminal do linux:
gcc -std=c11 -Wall -Wextra main.c jogador.c mapa.c eventos.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./jogo
*/ 

#include <raylib.h>
#include <string.h> // Permite usar strcpy para manipular textos
#include "jogo.h"

int main() {
    int resultado = EM_ANDAMENTO;
    int reiniciar = 0;
    Jogo jogo;

    // Tenta abrir o mapa inicial. Se não achar o arquivo, o programa fecha com erro (return 1)
    if (carregarMapa("mapa1.txt", &jogo) == 0) {
        return 1; 
    }
    
    // Configura o estado inicial do jogador
    jogo.energia = ENERGIA_INICIAL;
    jogo.cristais = 0;
    jogo.turnos = 0;
    strcpy(jogo.mensagem, "Explore a ilha!"); // strcpy copia a frase para dentro da variável

    InitWindow(800, 600, "Expedicao na Ilha dos Algoritmos");
    SetTargetFPS(30); // IMPRESCINDÍVEL: Trava o jogo em 30 frames para não sobrecarregar o CPU

    // LAÇO PRINCIPAL: Roda 30 vezes por segundo enquanto a janela estiver aberta
    while (!WindowShouldClose()) { 
        int comando = GetKeyPressed(); // Captura qual seta do teclado foi apertada

        // SE O JOGADOR DECIDIU RECOMEÇAR A PARTIDA
        if(reiniciar == 1) {
            carregarMapa("mapa1.txt", &jogo); // Lê o mapa do zero novamente
            jogo.energia = ENERGIA_INICIAL;
            jogo.cristais = 0;
            jogo.turnos = 0;
            strcpy(jogo.mensagem, "O jogo recomecou!");
            resultado = EM_ANDAMENTO;
            reiniciar = 0;
        }

        // LÓGICA DE MOVIMENTO E REGRAS (Só roda se o jogo não tiver acabado)
        if(resultado == EM_ANDAMENTO) {
            
            // Verifica se alguma seta foi apertada
            if(comando == KEY_UP || comando == KEY_DOWN || comando == KEY_LEFT || comando == KEY_RIGHT) {
                // Pergunta à função mapa.c se o jogador não vai bater na parede
                if(movimentoValido(&jogo, comando) == 1) {
                    
                    if(jogo.energia > 0) {
                        moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando); // Anda
                        jogo.energia--;     // Gasta 1 de energia
                        jogo.turnos++;      // Passa 1 turno
                        processarCasa(&jogo); // Verifica o que tinha no chão (Armadilha, Cristal, etc)
                        
                        // Checa se morreu de cansaço no meio do caminho
                        if(jogo.energia <= 0 && !chegouNaSaida(&jogo)) {
                            resultado = DERROTA;
                        }
                    }
                }
            }
            
            // Tecla de desistência
            if(comando == KEY_Q) {
                resultado = DESISTENCIA;
            }

            // Checa Condição de Vitória (Pisou no 'S' e tem cristais suficientes)
            if(chegouNaSaida(&jogo)) {
                if(jogo.cristais >= CRISTAIS_VITORIA) {
                    resultado = VITORIA;
                } else {
                    strcpy(jogo.mensagem, "Faltam cristais!");
                }
            }
        }

        // RENDERIZAÇÃO: Tudo que for desenhado na tela TEM que ficar entre Begin e End
        if (resultado == EM_ANDAMENTO) {
            BeginDrawing();
            ClearBackground(RAYWHITE); // Limpa o frame anterior pintando tudo de branco
            desenharJogo(&jogo);       // Chama a função que desenha a matriz
            EndDrawing();
        } else {
            // Se o jogo acabou, a função mostrarFimDeJogo assume o controle da tela.
            // Ela devolve 1 se o jogador apertar 'S' (Sim, quero jogar de novo)
            if(mostrarFimDeJogo(&jogo, resultado) == 1) {
                reiniciar = 1;
            } else {
                break; // Se apertar 'N', o 'break' quebra o laço e o jogo fecha
            }
        }
    }

    CloseWindow(); // Desliga a placa de vídeo
    return 0;
}