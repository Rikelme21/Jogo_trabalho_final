#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "jogo.h"

int carregarMapa(const char *nomeArquivo, Jogo *j, int nivelDesejado) {
    FILE *ficheiro = fopen(nomeArquivo, "r");
    if (ficheiro == NULL) return 0; 
    
    int nivelAtual = 1;
    char linhaTemp[MAX_COLUNAS];

    while (nivelAtual < nivelDesejado) {
        if (fgets(linhaTemp, MAX_COLUNAS, ficheiro) == NULL) {
            fclose(ficheiro);
            return 0; 
        }
        if (linhaTemp[0] == '-') nivelAtual++;
    }

    int linhaMatriz = 0;
    while (fgets(j->mapa[linhaMatriz], MAX_COLUNAS, ficheiro) != NULL) {
        if (j->mapa[linhaMatriz][0] == '-') break; 
        
        for (int colunaAtual = 0; j->mapa[linhaMatriz][colunaAtual] != '\0'; colunaAtual++) {
            if (j->mapa[linhaMatriz][colunaAtual] == 'P') {
                j->linhaJ = linhaMatriz;
                j->colunaJ = colunaAtual;
                j->mapa[linhaMatriz][colunaAtual] = '.'; 
            }
        }
        linhaMatriz++;
        if (linhaMatriz >= MAX_LINHAS) break; 
    }

    if (linhaMatriz == 0) {
        fclose(ficheiro);
        return 0;
    }

    j->linhas = linhaMatriz;

    int contaColunas = 0;
    while (j->mapa[0][contaColunas] != '\n' && j->mapa[0][contaColunas] != '\0' && j->mapa[0][contaColunas] != '\r') {
        contaColunas++;
    }
    j->colunas = contaColunas;

    fclose(ficheiro);
    return 1; 
}

int movimentoValido(Jogo *j, int comando) {
    int novaLinha = j->linhaJ;
    int novaColuna = j->colunaJ;

    if(comando == KEY_UP) novaLinha--;
    else if(comando == KEY_DOWN) novaLinha++;
    else if(comando == KEY_LEFT) novaColuna--;
    else if(comando == KEY_RIGHT) novaColuna++;
    else return 0; 
    
    if(novaLinha < 0 || novaLinha >= j->linhas || novaColuna < 0 || novaColuna >= j->colunas) {
        strcpy(j->mensagem, "Limite do mapa!"); 
        return 0;
    }
    if(j->mapa[novaLinha][novaColuna] == '#') {
        strcpy(j->mensagem, "Bateu na parede!"); 
        return 0;
    }
    return 1;
}

void desenharJogo(Jogo *j) {
    
    int larguraMapa = j->colunas * 41;
    int alturaMapa = j->linhas * 41;
    int larguraPainel = 300; 
    int espacoMeio = 50; 
    
    int larguraTotal = larguraMapa + espacoMeio + larguraPainel;
    
    //Descobre o X e Y exatos para que o bloco inteiro fique no meio de 1280x720
    int offsetX = (1280 - larguraTotal) / 2;
    int offsetY = (720 - alturaMapa) / 2;

    //Garante que o mapa não fica colado ao teto se for muito grande
    if (offsetY < 40) offsetY = 40; 
    if (offsetX < 40) offsetX = 40;

    //DESENHO DA MATRIZ
    for(int op1 = 0; op1 < j->colunas; op1++) {
        for(int op2 = 0; op2 < j->linhas; op2++) {
            
            int posX = offsetX + (op1 * 41);
            int posY = offsetY + (op2 * 41);

            DrawRectangle(posX, posY, 40, 40, LIGHTGRAY);

            if(j->mapa[op2][op1]=='#') DrawRectangle(posX, posY, 40, 40, DARKGRAY);
            if(j->mapa[op2][op1]=='C') {
                DrawCircle(posX + 20, posY + 20, 15, GOLD);  
                DrawText("$", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='R') DrawRectangle(posX, posY, 40, 40, GREEN); 
            if(j->mapa[op2][op1]=='A') DrawRectangle(posX, posY, 40, 40, RED);   
            if(j->mapa[op2][op1]=='E') DrawRectangle(posX, posY, 40, 40, ORANGE);
            if(j->mapa[op2][op1]=='S') DrawRectangle(posX, posY, 40, 40, PURPLE);
        }
    }

    //DESENHO DO JOGADOR
    int posJogX = offsetX + (j->colunaJ * 41) + 3;
    int posJogY = offsetY + (j->linhaJ * 41) + 3;
    DrawRectangle(posJogX, posJogY, 34, 34, BLUE);

    //PAINEL DE INFORMAÇÕES
    int painelX = offsetX + larguraMapa + espacoMeio;
    int topoY = offsetY;

    DrawText("PAINEL DO EXPLORADOR", painelX, topoY, 20, DARKGRAY);
    DrawText(TextFormat("Jogador: %s", j->nomeJogador), painelX, topoY + 40, 20, BLUE); 
    DrawText(TextFormat("Fase: %d", j->nivel), painelX, topoY + 70, 20, MAGENTA); 
    
    DrawText(TextFormat("Energia: %d", j->energia), painelX, topoY + 120, 20, BLACK);
    DrawText(TextFormat("Cristais: %d / %d", j->cristais, CRISTAIS_VITORIA), painelX, topoY + 150, 20, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos), painelX, topoY + 180, 20, BLACK);
    
    DrawText("Status:", painelX, topoY + 230, 20, DARKGRAY);
    DrawText(j->mensagem, painelX, topoY + 260, 20, DARKGREEN);

    DrawText("Pressione 'Q' para desistir", painelX, 680, 15, GRAY);
}