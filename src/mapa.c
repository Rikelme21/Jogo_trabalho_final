#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jogo.h"

//verifica se o moviemnto é valido, retorna 1 quando for, se não, retorna 0
int movimentoValido(Jogo *j, int comando){
    int novaLinha = j->linhaJ;
    int novaColuna = j->colunaJ;

    //calcula a linha desejada pelo jogador
    if(comando == KEY_UP){
        novaLinha = novaLinha - 1;
    }else if(comando == KEY_DOWN){
        novaLinha = novaLinha + 1;
    }else if(comando == KEY_LEFT){
        novaColuna = novaColuna - 1;
    }else if(comando == KEY_RIGHT){
        novaColuna = novaColuna + 1;
    }else{
        return 0;
    }
    
    //verifica se é valido o movimento
    if(novaLinha < 0 || novaLinha >= j->linhas || novaColuna < 0 || novaColuna >= j->colunas){
        return 0;
    }
    if(j->mapa[novaLinha][novaColuna] == '#'){
        return 0;
    }

    return 1;
}

void desenharJogo(Jogo *j){
    int op1, op2;

    //desenha o mapa
    for(op1 = 0; op1 < j->colunas; op1++){
        for(op2 = 0; op2 < j->linhas; op2++){
            DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, GRAY);

            if(j->mapa[op2][op1]=='#'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, BLACK);
            }
            if(j->mapa[op2][op1]=='C'){
                DrawCircle((op1 * 41)+60, (op2 * 41)+60, (35 / 2), YELLOW);
                DrawText(TextFormat("$"),(op1 * 41)+55, (op2 * 41)+52, 20, BLACK);
            }
            if(j->mapa[op2][op1]=='R'){
                DrawRectangle((op1 * 41)+43, (op2 * 41)+43, 34, 34, GREEN);
                DrawText(TextFormat("+"),(op1 * 41)+55, (op2 * 41)+52, 20, BLACK);

            }
            if(j->mapa[op2][op1]=='A'){
                DrawRectangle((op1 * 41)+43, (op2 * 41)+43, 34, 34, RED);
                DrawText(TextFormat("x"),(op1 * 41)+55, (op2 * 41)+50, 20, BLACK);

            }
            if(j->mapa[op2][op1]=='E'){
                DrawRectangle((op1 * 41)+43, (op2 * 41)+43, 34, 34, ORANGE);
                DrawText(TextFormat("?"),(op1 * 41)+55, (op2 * 41)+52, 20, BLACK);

            }
            if(j->mapa[op2][op1]=='S'){
                DrawRectangle((op1 * 41)+43, (op2 * 41)+43, 34, 34, PURPLE);
                DrawText(TextFormat("S"),(op1 * 41)+55, (op2 * 41)+52, 20, BLACK);

            }
            
        }
    }

    //textos
    DrawText(TextFormat("Energia: %d", j->energia),50, 420, 20, BLACK);
    DrawText(TextFormat("Cristais: %d / %d", j->cristais, j->minimoCristais),50, 450, 20, BLACK);
    DrawText(TextFormat("Turnos: %d", j->turnos),50, 480, 20, BLACK);
    DrawText(TextFormat("Pressione 'Q' para desistir"),50, 540, 20, BLACK);
    
    DrawText("P: Player",480, 40, 20, BLACK);
    DrawText("$: Cristais",480, 70, 20, BLACK);
    DrawText("+: Recuperação",480, 100, 20, BLACK);
    DrawText("?: Evento misterioso",480, 130, 20, BLACK);
    DrawText("x: Armadilha",480, 160, 20, BLACK);
    DrawText("S: Saída",480, 190, 20, BLACK);

    DrawText("Use as setas para movimentar",480, 250, 20, BLACK);
    DrawText("o player",480, 270, 20, BLACK);

    DrawText(j->mensagem, 50, 390, 20, BLACK);

    //desenha boneco
    DrawRectangle((j->colunaJ * 41)+43, (j->linhaJ * 41)+43, 34, 34, BLUE);
    DrawText(TextFormat("P"),(j->colunaJ * 41)+55, (j->linhaJ * 41)+52, 20, BLACK);

    
}

int carregarMapa(const char *nomeArquivo, Jogo *j){
    char texto[100];
    int op1, op2, quantPlayer = 0, quantSaida = 0, quantCristais = 0;

    //ao carregar o mapa zera os cristais e os turnos
    j->cristais = 0;
    j->turnos = 0;

    //carrega o arquivo
    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        strcpy(j->mensagem, "O mapa não pode ser carregado");
        return 0;
    }

    if(fgets(texto, sizeof(texto), arquivo) == NULL){
        strcpy(j->mensagem, "Não foi possivel ler a linha");
        fclose(arquivo);
        return 0;
    }
    if (sscanf(texto, "%d %d", &j->linhas, &j->colunas) != 2) {
        strcpy(j->mensagem, "As dimensões são invalidas");
        fclose(arquivo);
        return 0;

    }
    if (j->linhas < 6 || j->linhas > MAX_LINHAS || j->colunas < 8 || j->colunas > MAX_COLUNAS){
        strcpy(j->mensagem, "As dimensões são invalidas, pois não atendem os requisitos minimos e maximos");
        fclose(arquivo);
        return 0;
    }

    if(fgets(texto, sizeof(texto), arquivo) == NULL){
        strcpy(j->mensagem, "Não foi possivel ler a segunda linha");
        fclose(arquivo);
        return 0;
    }
    if(sscanf(texto, "%d %d", &j->energia, &j->minimoCristais) != 2) {
        strcpy(j->mensagem, "As configurações são invalidas");
        fclose(arquivo);
        return 0;
    }
    if (j->energia < 1 || j->minimoCristais < 0){
        strcpy(j->mensagem, "As configurações são invalidas, erro na energia inicial ou cristais mimos");
        fclose(arquivo);
        return 0;
    }

    for(op1 = 0; op1 < j->linhas; op1++) {
        if(fgets(texto, sizeof(texto), arquivo) == NULL){
            strcpy(j->mensagem, "Não foi possivel ler o mapa");
            fclose(arquivo);
            return 0;
        }
        texto[strcspn(texto, "\r\n")] = '\0';
        if (strlen(texto) != (size_t)j->colunas) {
            strcpy(j->mensagem, "Não foi possivel ler o mapa, tamanho de colnas");
            fclose(arquivo);
            return 0;
        }
        for(op2 = 0; op2 < j->colunas; op2++){
            j->mapa[op1][op2] = texto[op2];
            if(j->mapa[op1][op2] == 'P'){
                j->linhaInicial = op1;
                j->colunaInicial = op2;
                j->linhaJ = op1;
                j->colunaJ = op2;
                j->mapa[op1][op2] = '.';
                quantPlayer++;
            }
            if(j->mapa[op1][op2] == 'S'){
                quantSaida++;
            }
            if(j->mapa[op1][op2] == 'C'){
                quantCristais++;
            }
            if(j->mapa[op1][op2] != '.' && j->mapa[op1][op2] != '#' && j->mapa[op1][op2] != 'S' &&
             j->mapa[op1][op2] != 'C' && j->mapa[op1][op2] != 'R' && j->mapa[op1][op2] != 'A' && j->mapa[op1][op2] != 'E'){
                strcpy(j->mensagem, "Não foi possivel ler o mapa, há caracteres que não são validos");
                fclose(arquivo);
                return 0;
            }
        }
    }
    if (fgets(texto, sizeof(texto), arquivo) != NULL) {
        // o arquivo contém uma linha extra
        strcpy(j->mensagem, "Não foi possivel ler o mapa, ha linha extras");
        fclose(arquivo);
        return 0;
    }   
    if(quantPlayer != 1 || quantSaida != 1 || quantCristais < j->minimoCristais || quantCristais < 3){
        strcpy(j->mensagem, "Não foi possivel ler o mapa, a quantidade de saidas não é 1");
        fclose(arquivo);
        return 0;
    }

    j->mensagem[0] = '\0';
    fclose(arquivo);
    return 1;
}

//configurações iniciais do jogo, não ultilizado na versão final
/*
void configuraMapa(Jogo *j){
    int op1, op2;

    j->mensagem[0] = '\0';

    j->linhaJ = 1; // inicia as variaveis
    j->colunaJ = 1;

    j->energia = ENERGIA_INICIAL;
    j->cristais = 0;
    j->turnos = 0;


    j->linhas = LINHAS;
    j->colunas = COLUNAS;

    for(op1 = 0; op1 < LINHAS; op1++){
        for(op2 = 0; op2 < COLUNAS; op2++){
            if(op1 == 0 || op1 == (LINHAS - 1) || op2 == 0 || op2 == (COLUNAS - 1)){
                j->mapa[op1][op2] = '#';
            }else{
                j->mapa[op1][op2] = '.';
            }
            
        }
    }

    j->mapa[1][2] = '#';
    j->mapa[2][2] = '#';
    j->mapa[2][3] = '#';
    j->mapa[2][4] = '#';
    j->mapa[3][4] = '#';
    j->mapa[4][1] = '#';
    j->mapa[5][3] = '#';
    j->mapa[5][4] = '#';
    j->mapa[6][4] = '#';
    j->mapa[2][7] = '#';
    j->mapa[2][6] = '#';
    j->mapa[3][6] = '#';
    j->mapa[4][6] = '#';
    j->mapa[2][8] = '#';
    

    j->mapa[5][3] = 'R';
    j->mapa[1][3] = 'R';

    j->mapa[5][7] = 'A';
    j->mapa[1][5] = 'E';


    j->mapa[2][5] = 'C';
    j->mapa[5][1] = 'C';
    j->mapa[6][3] = 'C';
    j->mapa[6][6] = 'C';

    j->mapa[3][8] = 'S';
}*/
