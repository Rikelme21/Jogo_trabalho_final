#include <raylib.h>
#include "jogo.h"

#include <raylib.h>
#include <string.h> // Para usar strcpy() nas mensagens
#include "jogo.h"

// Verifica em qual letra o jogador pisou e aplica a regra
void processarCasa(Jogo *j) {
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
        strcpy(j->mensagem, "Armadilha! Perdeu 4 de energia.");
    }
    else if (casaAtual == 'E') {
        // Evento surpresa: Dá energia mas custa 2 turnos de penalidade
        j->energia += 2;
        j->turnos += 2; 
        strcpy(j->mensagem, "Evento: Comeu uma fruta magica.");
    }
    else if (casaAtual == 'S') {
        strcpy(j->mensagem, "Chegou na saida!");
    }
    else {
        strcpy(j->mensagem, "Caminho livre...");
    }

    // Depois do jogador passar pela casa, ela vira chão vazio ('.').
    // O if garante que a Letra 'S' (Saída) não seja apagada do mapa.
    if (casaAtual != 'S') {
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

// Monta a tela preta e branca de fim de jogo
int mostrarFimDeJogo(Jogo *j, int resultado) {
    int escolheu = 0;
    int opcao = 0;

    // Cria um sub-laço: O jogo fica "preso" aqui até o jogador escolher Sim(S) ou Não(N)
    while (escolheu == 0) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if(resultado == VITORIA) {
            DrawText("VITORIA!", 100, 100, 80, GREEN);
            DrawText("Voce escapou da ilha!", 100, 190, 30, DARKGREEN);
        }
        else if(resultado == DERROTA) {
            DrawText("DERROTA", 100, 100, 80, RED);
            DrawText("Sua energia acabou...", 100, 190, 30, MAROON);
        }
        else if(resultado == DESISTENCIA) {
            DrawText("DESISTENCIA", 100, 100, 80, ORANGE);
            DrawText("Voce abandonou a expedicao.", 100, 190, 30, DARKGRAY);
        }
        
        // Placar Final
        DrawText(TextFormat("Energias restantes: %d", j->energia), 100, 260, 30, BLACK);
        DrawText(TextFormat("Cristais: %d", j->cristais), 100, 300, 30, BLACK);
        DrawText(TextFormat("Turnos: %d", j->turnos), 100, 340, 30, BLACK);

        DrawText("Jogar novamente?", 100, 420, 30, BLACK);
        DrawText("Pressione 'S' para SIM, e 'N' para NAO", 100, 460, 20, GRAY);

        EndDrawing();

        // Aguarda a resposta do teclado
        if(IsKeyPressed(KEY_S)) {
            opcao = 1; // 1 significa que quer reiniciar
            escolheu = 1;
        } else if(IsKeyPressed(KEY_N)) {
            opcao = 0; // 0 significa que quer fechar
            escolheu = 1;
        }

        // Proteção caso o jogador clique no "X" da janela para forçar a saída
        if (WindowShouldClose()) {
            return 0;
        }
    }
    
    return opcao;
}