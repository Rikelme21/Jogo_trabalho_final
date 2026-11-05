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
