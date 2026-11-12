#include <raylib.h>
#include "jogo.h"

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
            if(j->mapa[op2][op1]=='.'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, GRAY);
            }
            if(j->mapa[op2][op1]=='#'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, BLACK);
            }
            if(j->mapa[op2][op1]=='C'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, YELLOW);
            }
            if(j->mapa[op2][op1]=='R'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, GREEN);
            }
            if(j->mapa[op2][op1]=='A'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, RED);
            }
            if(j->mapa[op2][op1]=='E'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, ORANGE);
            }
            if(j->mapa[op2][op1]=='S'){
                DrawRectangle((op1 * 41)+40, (op2 * 41)+40, 40, 40, PURPLE);
            }
        }
    }

    //textos
    DrawText(TextFormat("Energia: %d", j->energia),50, 450, 20, BLACK);
    DrawText(TextFormat("Cristais: %d", j->cristais),50, 480, 20, BLACK);
    DrawText(TextFormat("Pressione 'Q' para desistir"),50, 540, 20, BLACK);


    //desenha boneco
    DrawRectangle((j->colunaJ * 41)+40, (j->linhaJ * 41)+40, 40, 40, BLUE);
}

//int carregarMapa(const char *nomeArquivo, Jogo *j){

//}
