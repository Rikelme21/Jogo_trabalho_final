#ifndef JOGO_H
#define JOGO_H

#define MAX_LINHAS 20
#define MAX_COLUNAS 40
#define TAM_MSG 80

/* resultados da partida */
#define EM_ANDAMENTO 0
#define VITORIA 1
#define DERROTA 2
#define DESISTENCIA 3

typedef struct {
    char mapa[MAX_LINHAS][MAX_COLUNAS]; /* dimensoes lidas do arquivo */
    int linhas, colunas;                
    int linhaJ, colunaJ;                /* posicao atual do jogador */
    int energia;                        /* energia inicial vem do arquivo */
    int minimoCristais;                 /* lido do arquivo */
    int cristais, turnos;               
    char mensagem[TAM_MSG];             /* ultima mensagem mostrada na tela */
} Jogo;

/* mapa.c */
int carregarMapa(const char *nomeArquivo, Jogo *j); /* 1 = ok, 0 = erro */
int telaCabe(Jogo *j); /* ncurses: 1 se mapa + painel cabem (Na Raylib não é tao necessário, mas faz parte do molde) */
void desenharJogo(Jogo *j);
int movimentoValido(Jogo *j, int comando);

/* jogador.c */
void moverJogador(int *linha, int *coluna, int comando);

/* eventos.c */
void processarCasa(Jogo *j); /* cristal/evento; atualiza mensagem */
int chegouNaSaida(Jogo *j);
int mostrarFimDeJogo(Jogo *j, int resultado);

#endif