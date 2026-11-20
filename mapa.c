#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "jogo.h"

int carregarMapa(const char *nomeArquivo, Jogo *j, int nivelDesejado) {
    FILE *ficheiro = fopen(nomeArquivo, "r");
    if (ficheiro == NULL) return 0;

    int nivelAtual = 1;
    char linhaTemp[MAX_COLUNAS + 4];

    // Avança até a fase desejada usando o separador '-'
    while (nivelAtual < nivelDesejado) {
        if (fgets(linhaTemp, sizeof(linhaTemp), ficheiro) == NULL) {
            fclose(ficheiro);
            return 0;
        }
        if (linhaTemp[0] == '-') nivelAtual++;
    }

    // 1ª linha do cabeçalho: linhas e colunas
    if (fgets(linhaTemp, sizeof(linhaTemp), ficheiro) == NULL ||
        sscanf(linhaTemp, "%d %d", &j->linhas, &j->colunas) != 2) {
        fclose(ficheiro);
        return 0;
    }

    // 2ª linha do cabeçalho: energia inicial e mínimo de cristais
    if (fgets(linhaTemp, sizeof(linhaTemp), ficheiro) == NULL ||
        sscanf(linhaTemp, "%d %d", &j->energia, &j->minimoCristais) != 2) {
        fclose(ficheiro);
        return 0;
    }

    // Valida limites do cabeçalho
    if (j->linhas <= 0 || j->linhas > MAX_LINHAS ||
        j->colunas <= 0 || j->colunas >= MAX_COLUNAS ||
        j->energia <= 0 || j->minimoCristais < 0) {
        fclose(ficheiro);
        return 0;
    }

    int achouJogador = 0;
    int achouSaida = 0;

    for (int linhaMatriz = 0; linhaMatriz < j->linhas; linhaMatriz++) {
        if (fgets(linhaTemp, sizeof(linhaTemp), ficheiro) == NULL || linhaTemp[0] == '-') {
            fclose(ficheiro);
            return 0;
        }

        // Remove '\r' e '\n' do final da linha
        linhaTemp[strcspn(linhaTemp, "\r\n")] = '\0';

        if ((int)strlen(linhaTemp) != j->colunas) {
            fclose(ficheiro);
            return 0;
        }

        for (int colunaAtual = 0; colunaAtual < j->colunas; colunaAtual++) {
            char c = linhaTemp[colunaAtual];

            if (c != '#' && c != '.' && c != 'P' && c != 'C' &&
                c != 'R' && c != 'A' && c != 'E' && c != 'S' && c != 'B') {
                fclose(ficheiro);
                return 0;
            }

            if (c == 'P') {
                j->linhaJ = linhaMatriz;
                j->colunaJ = colunaAtual;
                j->linhaInicial = linhaMatriz;
                j->colunaInicial = colunaAtual;
                c = '.';
                achouJogador = 1;
            } else if (c == 'S') {
                achouSaida = 1;
            }

            j->mapa[linhaMatriz][colunaAtual] = c;
        }
        j->mapa[linhaMatriz][j->colunas] = '\0';
    }

    fclose(ficheiro);

    if (!achouJogador || !achouSaida) {
        return 0;
    }

    return 1;
}

int movimentoValido(Jogo *j, int comando) {
    int novaLinha = j->linhaJ;
    int novaColuna = j->colunaJ;
    int colunaCaixa, linhaCaixa;

    if (comando == KEY_UP || comando == KEY_W) novaLinha--;
    else if (comando == KEY_DOWN || comando == KEY_S) novaLinha++;
    else if (comando == KEY_LEFT || comando == KEY_A) novaColuna--;
    else if (comando == KEY_RIGHT || comando == KEY_D) novaColuna++;
    else return 0; 
    
    if (novaLinha < 0 || novaLinha >= j->linhas || novaColuna < 0 || novaColuna >= j->colunas) {
        strcpy(j->mensagem, "Limite do mapa!"); 
        return 0;
    }
    if (j->mapa[novaLinha][novaColuna] == '#') {
        strcpy(j->mensagem, "Bateu na parede!"); 
        return 0;
    }
    if(j->mapa[novaLinha][novaColuna] == 'B'){
        colunaCaixa = novaColuna;
        linhaCaixa = novaLinha;

        if (comando == KEY_UP || comando == KEY_W) linhaCaixa--;
        if (comando == KEY_DOWN || comando == KEY_S) linhaCaixa++;
        if (comando == KEY_LEFT || comando == KEY_A) colunaCaixa--;
        if (comando == KEY_RIGHT || comando == KEY_D) colunaCaixa++;

        if (linhaCaixa < 0 || linhaCaixa >= j->linhas) {
            strcpy(j->mensagem, "Caixa bateu no limite do mapa!"); 
            return 0;
        }
        if (j->mapa[linhaCaixa][colunaCaixa] == '#') {
            strcpy(j->mensagem, "Caixa bateu na parede!"); 
            return 0;
        }
        if (j->mapa[linhaCaixa][colunaCaixa] == 'S') {
            strcpy(j->mensagem, "Caixa bateu na saida!"); 
            return 0;
        }
        if (j->mapa[linhaCaixa][colunaCaixa] == 'B') {
            strcpy(j->mensagem, "Você não tem força para mover duas caixas!"); 
            return 0;
        }
    }
    return 1;
}

void desenharLegenda(int x, int y) {
    DrawText("LEGENDA:", x, y, 18, DARKGRAY);

    // Jogador
    DrawRectangle(x, y + 28, 18, 18, BLUE);
    DrawText("Jogador", x + 28, y + 28, 18, BLACK);

    // Cristal
    DrawCircle(x + 9, y + 63, 9, GOLD);
    DrawText("$", x + 5, y + 56, 15, BLACK);
    DrawText("Cristal", x + 28, y + 54, 18, BLACK);

    // Recarga
    DrawRectangle(x, y + 80, 18, 18, GREEN);
    DrawText(TextFormat("+"), x + 5, y + 81, 18, BLACK);
    DrawText("Recarga +4", x + 28, y + 80, 18, BLACK);

    // Armadilha
    DrawRectangle(x, y + 106, 18, 18, RED);
    DrawText(TextFormat("x"), x + 5, y + 106, 18, BLACK);
    DrawText("Armadilha -4", x + 28, y + 106, 18, BLACK);

    // Evento
    DrawRectangle(x, y + 132, 18, 18, ORANGE);
    DrawText(TextFormat("?"), x + 4, y + 133, 18, BLACK);
    DrawText("Evento Surpresa (E)", x + 28, y + 132, 18, BLACK);

    // Saida
    DrawRectangle(x, y + 158, 18, 18, PURPLE);
    DrawText(TextFormat("S"), x + 4, y + 159, 18, BLACK);
    DrawText("Saida", x + 28, y + 158, 18, BLACK);

    // Caixa
    DrawRectangle(x, y + 184, 18, 18, BROWN);
    DrawText("Caixa", x + 28, y + 184, 18, BLACK);
    DrawRectangle(x, y + 188, 18, 2, DARKBROWN);
    DrawRectangle(x, y + 192, 18, 2, DARKBROWN);
    DrawRectangle(x, y + 196, 18, 2, DARKBROWN);

    // Parede
    DrawRectangle(x, y + 210, 18, 18, DARKGRAY);
    DrawText("Parede", x + 28, y + 210, 18, BLACK);
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

            if(j->mapa[op2][op1]=='#'){
                DrawRectangle(posX, posY, 40, 40, DARKGRAY);
            }
            if(j->mapa[op2][op1]=='C') {
                DrawCircle(posX + 20, posY + 20, 15, GOLD);  
                DrawText("$", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='R'){
                DrawRectangle(posX + 3, posY + 3, 34, 34, GREEN); 
                DrawText("+", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='A'){
                DrawRectangle(posX + 3, posY + 3, 34, 34, RED);
                DrawText("x", posX + 15, posY + 10, 20, BLACK);

            }
            if(j->mapa[op2][op1]=='E'){ 
                DrawRectangle(posX + 3, posY + 3, 34, 34, ORANGE);
                DrawText("?", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='S'){
                DrawRectangle(posX + 3, posY + 3, 34, 34, PURPLE);
                DrawText("S", posX + 15, posY + 10, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='B'){
                DrawRectangle(posX + 1, posY + 1, 38, 38, BROWN);
                DrawRectangle(posX + 1, posY + 7, 38, 5, DARKBROWN);
                DrawRectangle(posX + 1, posY + 17, 38, 5, DARKBROWN);
                DrawRectangle(posX + 1, posY + 27, 38, 5, DARKBROWN);
            }
        }
    }

    //DESENHO DO JOGADOR
    int posJogX = offsetX + (j->colunaJ * 41) + 3;
    int posJogY = offsetY + (j->linhaJ * 41) + 3;
    DrawRectangle(posJogX, posJogY, 34, 34, BLUE);

    //PAINEL DE INFORMAÇÕES
    int painelX = offsetX + larguraMapa + espacoMeio;
    int topoY = 50;

    DrawText("PAINEL DO EXPLORADOR", painelX, topoY, 20, DARKGRAY);
    DrawText(TextFormat("Jogador: %s", j->nomeJogador), painelX, topoY + 35, 20, BLUE); 
    DrawText(TextFormat("Fase: %d", j->nivel), painelX, topoY + 65, 20, MAGENTA); 
    
    DrawText(TextFormat("Energia: %d", j->energia), painelX, topoY + 110, 20, BLACK);
    DrawText(TextFormat("Cristais: %d / %d", j->cristais, j->minimoCristais), painelX, topoY + 140, 20, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos), painelX, topoY + 170, 20, BLACK);
    
    DrawText("Status:", painelX, topoY + 215, 20, DARKGRAY);
    DrawText(j->mensagem, painelX, topoY + 245, 18, DARKGREEN);

    desenharLegenda(painelX, topoY + 295);

    DrawText("Mover: Setas ou WASD", painelX, 625, 16, DARKGRAY);
    DrawText("Pressione 'Q' para desistir", painelX, 650, 16, GRAY);
}