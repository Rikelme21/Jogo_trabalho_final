#include <raylib.h>

// o '*' indica que é um ponteiro, permitindo alterar o vaor armazenado no endereço da memoria
void moverJogador(int *linha, int *coluna, int comando){

    //bloco de comandos que troca o jogador de posição na matriz de acordo com a tecla pressionada
    if(comando == KEY_UP){
        *linha = *linha - 1;
    }else if(comando == KEY_DOWN){
        *linha = *linha + 1;
    }else if(comando == KEY_LEFT){
        *coluna = *coluna - 1;
    }else if(comando == KEY_RIGHT){
        *coluna = *coluna + 1;
    }
    
    
}