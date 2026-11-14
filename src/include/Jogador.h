/**
 * Definição do tipo Jogador e operações associadas.
 * 
 * Um Jogador representa, obviamente, o jogador do Breakout, que tem como
 * objetivo quebrar todos os alvos contidos na tela.
 */
#pragma once
#include "raylib/raylib.h"

typedef struct Jogador {
    Rectangle ret;
    float     velocidadeBase;    /* velocidade máxima (px/s) */
    float     velocidadeAtual;   /* velocidade no frame atual */
    Color     cor;
} Jogador;

void entradaJogador  ( Jogador *j );
void atualizarJogador( Jogador *j, float delta );
void desenharJogador ( Jogador *j );
