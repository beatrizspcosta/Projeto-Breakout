/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdbool.h>
#include "Jogador.h"
#include "Bolinha.h"
#include "Alvo.h"

#define COLUNAS_ALVOS   8
#define LINHAS_ALVOS    8
#define TOTAL_ALVOS     (COLUNAS_ALVOS * LINHAS_ALVOS)
#define VIDAS_INICIAIS  3

typedef enum EstadoJogo {
    ESTADO_AGUARDANDO,   //pausado esperando SPACE para iniciar/reiniciar
    ESTADO_JOGANDO,
    ESTADO_VITORIA,
    ESTADO_GAME_OVER
} EstadoJogo;

typedef struct GameWorld {

    Jogador     jogador;
    Bolinha     bolinha;
    Alvo        alvos[TOTAL_ALVOS];
    int         vidas;
    int         pontuacao;
    int         alvosRestantes;
    EstadoJogo  estado;

    //bools para verificar se aumenta velocidade da bolinha
    bool usouVelocidade240;
    bool usouVelocidade480;

} GameWorld;

GameWorld *createGameWorld( void );
void       destroyGameWorld( GameWorld *gw );
void       updateGameWorld( GameWorld *gw, float delta );
void       drawGameWorld( GameWorld *gw );