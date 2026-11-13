#ifndef JOGO_H
#define JOGO_H

#define MAX_LINHAS 20
#define MAX_COLUNAS 40
#define TAM_MSG 80
#define CRISTAIS_VITORIA 3
#define ENERGIA_INICIAL 20

#define TELA_INICIAL -1
#define EM_ANDAMENTO 0
#define VITORIA 1
#define DERROTA 2
#define DESISTENCIA 3

typedef struct {
    char mapa[MAX_LINHAS][MAX_COLUNAS]; 
    int linhas, colunas;                
    int linhaJ, colunaJ;                
    int energia;                        
    int minimoCristais;                 
    int cristais, turnos;               
    char mensagem[TAM_MSG];             
    char nomeJogador[50];               
    int nivel;                         
} Jogo;

int carregarMapa(const char *nomeArquivo, Jogo *j, int nivelDesejado); 
void desenharJogo(Jogo *j);
int movimentoValido(Jogo *j, int comando);

void moverJogador(int *linha, int *coluna, int comando);
void processarCasa(Jogo *j); 
int chegouNaSaida(Jogo *j);
int mostrarFimDeJogo(Jogo *j, int resultado);

#endif