#include <raylib.h>
#include <stdio.h>
#include <string.h> 
#include "jogo.h"

// Abre o ficheiro mapa1.txt e converte para a matriz do jogo
int carregarMapa(const char *nomeArquivo, Jogo *j) {
    FILE *ficheiro = fopen(nomeArquivo, "r");
    if (ficheiro == NULL) return 0; 
    
    int linhaAtual = 0;
    
    while (fgets(j->mapa[linhaAtual], MAX_COLUNAS, ficheiro) != NULL) {
        for (int colunaAtual = 0; j->mapa[linhaAtual][colunaAtual] != '\0'; colunaAtual++) {
            if (j->mapa[linhaAtual][colunaAtual] == 'P') {
                j->linhaJ = linhaAtual;
                j->colunaJ = colunaAtual;
                j->mapa[linhaAtual][colunaAtual] = '.'; 
            }
        }
        linhaAtual++;
        if (linhaAtual >= MAX_LINHAS) break; 
    }

    j->linhas = linhaAtual;

    int contaColunas = 0;
    while (j->mapa[0][contaColunas] != '\n' && j->mapa[0][contaColunas] != '\0' && j->mapa[0][contaColunas] != '\r') {
        contaColunas++;
    }
    j->colunas = contaColunas;

    fclose(ficheiro);
    return 1; 
}

// Checa colisões
int movimentoValido(Jogo *j, int comando) {
    int novaLinha = j->linhaJ;
    int novaColuna = j->colunaJ;

    if(comando == KEY_UP) novaLinha--;
    else if(comando == KEY_DOWN) novaLinha++;
    else if(comando == KEY_LEFT) novaColuna--;
    else if(comando == KEY_RIGHT) novaColuna++;
    else return 0; 
    
    //Não pode sair fora dos limites da matriz
    if(novaLinha < 0 || novaLinha >= j->linhas || novaColuna < 0 || novaColuna >= j->colunas) {
        strcpy(j->mensagem, "Limite do mapa!"); //Avisa que tentou sair da tela
        return 0;
    }
    
    //Não pode atravessar parede
    if(j->mapa[novaLinha][novaColuna] == '#') {
        strcpy(j->mensagem, "Bateu na parede!"); //Avisa que bateu no obstáculo
        return 0;
    }

    return 1; //Movimento liberado
}

// Desenha visualmente a matriz na tela usando formas geométricas
void desenharJogo(Jogo *j) {
    
    //DESENHO DA MATRIZ
    for(int op1 = 0; op1 < j->colunas; op1++) {
        for(int op2 = 0; op2 < j->linhas; op2++) {
            
            int posX = (op1 * 41) + 40;
            int posY = (op2 * 41) + 40;

            DrawRectangle(posX, posY, 40, 40, LIGHTGRAY); // Chão base

            if(j->mapa[op2][op1]=='#') {
                DrawRectangle(posX, posY, 40, 40, DARKGRAY); // Parede
            }
            if(j->mapa[op2][op1]=='C') {
                DrawCircle(posX + 20, posY + 20, 15, GOLD);  // Cristal redondo
                DrawText("$", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='R') {
                DrawRectangle(posX, posY, 40, 40, GREEN); // Recupera Energia
            }
            if(j->mapa[op2][op1]=='A') {
                DrawRectangle(posX, posY, 40, 40, RED);   // Armadilha
            }
            if(j->mapa[op2][op1]=='E') {
                DrawRectangle(posX, posY, 40, 40, ORANGE);// Evento
            }
            if(j->mapa[op2][op1]=='S') {
                DrawRectangle(posX, posY, 40, 40, PURPLE);// Saída
            }
        }
    }

    //DESENHO DO JOGADOR
    int posJogX = (j->colunaJ * 41) + 43;
    int posJogY = (j->linhaJ * 41) + 43;
    DrawRectangle(posJogX, posJogY, 34, 34, BLUE);

    // 3. PAINEL DE INFORMAÇÕES
    DrawText("PAINEL DO EXPLORADOR", 500, 40, 20, DARKGRAY);
    
    //Mostra o nome do jogador digitado na tela inicial
    DrawText(TextFormat("Jogador: %s", j->nomeJogador), 500, 70, 20, BLUE); 
    
    DrawText(TextFormat("Energia: %d", j->energia), 500, 100, 20, BLACK);
    DrawText(TextFormat("Cristais: %d / %d", j->cristais, CRISTAIS_VITORIA), 500, 130, 20, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos), 500, 160, 20, BLACK);
    
    DrawText("Status:", 500, 220, 20, DARKGRAY);
    DrawText(j->mensagem, 500, 250, 20, DARKGREEN);

    DrawText("Pressione 'Q' para desistir", 500, 500, 15, GRAY);
}